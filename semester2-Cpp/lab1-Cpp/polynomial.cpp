#include "polynomial.h"
#include <iostream>

Polynomial::Polynomial() {
    n = 0;
    maxDegree = 0;
}

Polynomial::Polynomial(int size, const double *givenCoefficients) {
    n = size;
    int md = 0;
    for (int i = 0; i < n; i++) {
        coefficients[i] = givenCoefficients[i];
        if (givenCoefficients[i] != 0) md = std::max(md, i);
    }
    maxDegree = md;
}

Polynomial::Polynomial(int size, const std::map<int, double>& givenCoefficients) {
    n = size;
    int md = 0;
    for (auto term : givenCoefficients) {
        md = std::max(md, term.first);
    }
    coefficients = givenCoefficients;
    maxDegree = md;
}

Polynomial::Polynomial(const Polynomial &polynomial) {
    n = polynomial.n;
    coefficients = polynomial.coefficients;
    maxDegree = polynomial.maxDegree;
}

Polynomial &Polynomial::operator=(const Polynomial &polynomial) {
    if (&polynomial == this) {
        return *this;
    }
    n = polynomial.n;
    coefficients = polynomial.coefficients;
    maxDegree = polynomial.maxDegree;
    return *this;
}

Polynomial::~Polynomial() = default;


int Polynomial::getN() const {
    return n;
}

std::map<int, double> Polynomial::getCoefficients() {
    return coefficients;
}

bool Polynomial::operator==(const Polynomial &polynomial) {
    return (this->n == polynomial.n && this->coefficients == polynomial.coefficients);
}

bool Polynomial::operator!=(const Polynomial &polynomial) {
    return !(Polynomial::operator==(polynomial));
}

Polynomial Polynomial::operator+(const Polynomial &polynomial) {
    Polynomial tmp1 = *this;
    Polynomial tmp2 = polynomial;
    std::map<int, double> sum;
    sum = tmp1.getCoefficients();
    for (auto term : tmp2.coefficients) {
        sum[term.first] = tmp1[term.first] + term.second;
    }
    Polynomial res(n + polynomial.getN(), sum);
    return res;
}

Polynomial Polynomial::operator-() const {
    Polynomial tmp(*this);
    for (auto term : coefficients) {
        tmp.coefficients[term.first] = -term.second;
    }
    tmp.n = this->getN();
    tmp.maxDegree = this->maxDegree;
    return tmp;
}

Polynomial Polynomial::operator-(const Polynomial &polynomial) {
    return (Polynomial::operator+(-polynomial));
}

Polynomial Polynomial::operator+=(const Polynomial &polynomial) {
    return (Polynomial::operator=(Polynomial::operator+(polynomial)));
}

Polynomial Polynomial::operator-=(const Polynomial &polynomial) {
    return (Polynomial::operator=(Polynomial::operator-(polynomial)));
}

Polynomial Polynomial::operator*(const Polynomial &polynomial) {
    Polynomial toMult = polynomial;
    std::map<int, double> tmp;
    int size = maxDegree + polynomial.maxDegree + 1;
    for (auto term1 : coefficients) {
        for (auto term2 : toMult.coefficients) {
            if (term1.second != 0 && term2.second != 0) {
                tmp[term1.first + term2.first] += term1.second * term2.second;
            }
        }
    }
    Polynomial mult(size, tmp);
    return mult;
}

Polynomial Polynomial::operator*(double number) {
    Polynomial tmp = *this;
    for (auto term : tmp.coefficients) {
        tmp.coefficients[term.first] = term.second * number;
    }
    return tmp;
}

Polynomial Polynomial::operator/(double number) {
    Polynomial tmp = *this;
    for (auto term : tmp.coefficients) {
        tmp.coefficients[term.first] = term.second / number;
    }
    return tmp;
}

Polynomial Polynomial::operator*=(const Polynomial &polynomial) {
    return (Polynomial::operator=(Polynomial::operator*(polynomial)));
}

Polynomial Polynomial::operator*=(double number) {
    return (Polynomial::operator=(Polynomial::operator*(number)));
}

Polynomial Polynomial::operator/=(double number) {
    return (Polynomial::operator=(Polynomial::operator/(number)));
}

std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial) {
    Polynomial tmp = polynomial;
    int counter = 0;
    bool flag = false;
    for (int i = tmp.getN() - 1; i >= 0; i--) {
        if (tmp.getCoefficients()[i] != 0) {
            if (i < tmp.getN() - 1 && tmp.getCoefficients()[i] > 0 && flag) {
                out << "+";
            }
            if (tmp.getCoefficients()[i] == 1 && i != 0) {

            } else if (tmp.getCoefficients()[i] == -1 && i != 0) {
                out << "-";
            } else {
                out << tmp.getCoefficients()[i];
            }
            if (i != 0) {
                out << "x";
                if (i != 1) {
                    out << "^" << i;
                }
                flag = true;
            }
        } else if (tmp.getN() == 1 && tmp.getCoefficients()[0] == 0) {
            out << "0";
        } else {
            counter++;
        }
    }
    if (counter == tmp.getN()) {
        out << "0";
    }
    return out;
}

std::istream &operator>>(std::istream &in, Polynomial &polynomial) {
    Polynomial tmp;
    std::string str;
    in >> str;
    int sign = 1;
    bool flagPower = false;
    bool flagTerm = false;
    std::string term;
    std::string power;
    for (int i = 0; i < str.length(); i++) {
        char current = str[i];
        if (current == '-' || current == '+') {
            if (i != 0 && str[i - 1] == '^') {
                power += current;
            } else {
                if (flagTerm) {
                    double coef;
                    if (term.empty()) {
                        coef = 1;
                    } else {
                        coef = stof(term);
                    }
                    if (power.empty()) {
                        tmp.coefficients[1] = coef * sign;
                    } else {
                        tmp.coefficients[stoi(power)] = coef * sign;
                    }
                    flagTerm = false;
                    flagPower = false;
                    power = "";
                    term = "";
                }
            }
            if (current == '-') {
                sign = -1;
            } else {
                sign = 1;
            }
        }
        if (isdigit(current)) {
            if (flagPower) {
                power += current;
            } else {
                term += current;
            }
        }
        if (current == '.' && !flagPower) {
            term += current;
        }
        if (current == 'x') {
            flagTerm = true;
            if (i + 1 < str.length() && str[i + 1] == '^') {
                flagPower = true;
            } else if (i + 1 >= str.length()) {
                double coef;
                if (term.empty()) {
                    coef = 1;
                } else {
                    coef = stof(term);
                }
                tmp.coefficients[1] = coef * sign;
                term = "";
                power = "";
            }
        }
    }
    if (!term.empty() || !power.empty()) {
        if (flagTerm) {
            double coef;
            if (term.empty()) {
                coef = 1;
            } else {
                coef = stof(term);
            }
            if (power.empty()) {
                tmp.coefficients[1] = coef * sign;
            } else {
                tmp.coefficients[stoi(power)] = coef * sign;
            }
        } else {
            tmp.coefficients[0] = stof(term) * (float)sign;
        }
    }
    int size = 0;
    for (auto i : tmp.coefficients) {
        size = std::max(size, i.first + 1);
    }
    Polynomial result(size, tmp.coefficients);
    polynomial = result;
    return in;
}

double Polynomial::operator[](int degree) {
    return coefficients[degree];
}