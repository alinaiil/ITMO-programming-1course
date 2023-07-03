#include <iostream>
#include <cmath>
#include <gtest/gtest.h>

enum pn {
    VALUE, DERIVATIVE, INTEGRAL
};

constexpr float pow(int x, int power) {
    float answer = 1;
    if (power < 0) {
        for (int i = 0; i < -power; i++) {
            answer /= (float) x;
        }
    } else if (power > 0) {
        for (int i = 0; i < power; i++) {
            answer *= (float) x;
        }
    }
    return answer;
}

constexpr bool isDigit(char a) {
    if (a == '0' || a == '1' || a == '2' || a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' ||
        a == '9') {
        return true;
    } else {
        return false;
    }
}

class Polynomial {
private:
    std::string_view pstr;
    int px;
    int pa;
    int pb;
public:
    constexpr Polynomial(const std::string_view &str, const int x, const int a, const int b) : pstr{str}, px{x}, pa{a},
                                                                                               pb{b} {}

    constexpr static float parse(const std::string_view &str, const int x, const int a, const int b, pn what) {
        bool flagPower = false;
        bool flagTerm = false;
        float term = 0;
        float power = 0;
        float sign = 1;
        float signPower = 1;
        float result = 0;
        float der = 0;
        float in = 0;
        for (int i = 0; i < str.length(); i++) {
            char current = str[i];
            if (current == '-' || current == '+') {
                if (i != 0 && str[i - 1] == '^') {
                    if (current == '-') {
                        signPower = -1;
                    } else {
                        signPower = 1;
                    }
                } else {
                    if (flagTerm) {
                        float coef = 0;
                        if (term == 0) {
                            coef = 1;
                        } else {
                            coef = term;
                        }
                        if (power == 0) {
                            power = 1;
                        }
                        result += coef * sign * pow(x, (int) (signPower * power));
                        der += signPower * power * coef * sign * pow(x, (int) (signPower * power - 1));
                        if (signPower * power == -1) {
                            in += coef * sign * ((float) (log(abs(b)) - log(abs(a))));
                        } else {
                            in += coef * sign * (pow(b, (int) (signPower * power + 1)) / (signPower * power + 1) -
                                                 pow(a, (int) (signPower * power + 1)) / (signPower * power + 1));
                        }
                        signPower = 1;
                        flagTerm = false;
                        flagPower = false;
                        power = 0;
                        term = 0;
                    }
                    if (current == '-') {
                        sign = -1;
                    } else {
                        sign = 1;
                    }
                }
            }
            if (isDigit(current)) {
                if (flagPower) {
                    if (power != 0) {
                        power *= 10;
                    }
                    power += (float) (current - '0');
                } else {
                    if (term != 0) {
                        term *= 10;
                    }
                    term += (float) (current - '0');
                    if (i + 2 < str.length() && (str[i + 2] == '+' || str[i + 2] == '-')) {
                        result += term * sign;
                        in += term * sign * (float) b - term * sign * (float) a;
                        term = 0;
                    }
                }
            }
            if (current == 'x') {
                flagTerm = true;
                if (i + 1 < str.length() && str[i + 1] == '^') {
                    flagPower = true;
                } else if (i + 1 >= str.length()) {
                    float coef = 0;
                    if (term == 0) {
                        coef = 1;
                    } else {
                        coef = term;
                    }
                    result += coef * sign * (float) x;
                    der += coef * sign;
                    in += coef * sign * ((pow(b, 2) / 2) - (pow(a, 2) / 2));
                    term = 0;
                    power = 0;
                }
            }
        }
        if (term != 0 || power != 0) {
            if (flagTerm) {
                float coef = 0;
                if (term == 0) {
                    coef = 1;
                } else {
                    coef = term;
                }
                if (power == 0) {
                    power = 1;
                }
                result += coef * sign * pow(x, (int) (signPower * power));
                der += signPower * power * coef * sign * pow(x, (int) (signPower * power - 1));
                if (signPower * power == -1) {
                    in += coef * sign * ((float) (log(abs(b)) - log(abs(a))));
                } else {
                    in += coef * sign * (pow(b, (int) (signPower * power + 1)) / (signPower * power + 1) -
                                         pow(a, (int) (signPower * power + 1)) / (signPower * power + 1));
                }
            } else {
                result += term * sign;
                in += term * sign * (float) (b - a);
            }
        }
        if (what == VALUE) {
            return result;
        } else if (what == DERIVATIVE) {
            return der;
        } else {
            return in;
        }
    }

    constexpr float getValue() const {
        return parse(pstr, px, pa, pb, VALUE);
    }

    constexpr float getDerivative() const {
        return parse(pstr, px, pa, pb, DERIVATIVE);
    }

    constexpr float getIntegral() const {
        return parse(pstr, px, pa, pb, INTEGRAL);
    }

    friend std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial) {
        std::string str;
        for (int i = 0; i < polynomial.pstr.length(); i++) {
            if (polynomial.pstr[i] == 'x') {
                if (i - 1 >= 0 && std::isdigit(polynomial.pstr[i - 1])) {
                    str += "*";
                }
                str += "(" + std::to_string((int) polynomial.px) + ")";
            } else {
                str += polynomial.pstr[i];
            }
        }
        out << str << " = " << polynomial.getValue() << ". " << "Derivative equals to: " << polynomial.getDerivative()
            << ". Definite integral from " << polynomial.pa << " to " << polynomial.pb << " equals to: "
            << polynomial.getIntegral();
        return out;
    }
};

TEST(PolynomialTest, TestOutput) {
    constexpr Polynomial test("8x^-2 - 10x^3 + 123", 2, 1, 3);
    std::stringstream polynomial;
    polynomial << test;
    EXPECT_EQ(polynomial.str(),
              "8*(2)^-2 - 10*(2)^3 + 123 = 45. Derivative equals to: -122. Definite integral from 1 to 3 equals to: 51.3333");
}

TEST(PolynomialTest, TestIntegral) {
    constexpr Polynomial test1("x^3 + 16x - 2", 8, 3, 5);
    EXPECT_EQ(260, test1.getIntegral());
    constexpr Polynomial test2("8x^9 + 10x^3 + 123", 2, 0, 1);
    EXPECT_EQ((float) 126.3, test2.getIntegral());
    constexpr Polynomial test3("x^5 + 19 + 3x^4 + 2x^3 + x^2 + x", 3, 1, 4);
    EXPECT_EQ((float) 1509.3, test3.getIntegral());
}

TEST(PolynomialTest, TestDerivative) {
    constexpr Polynomial test1("x^3 + 16x - 2", 8, 1, 3);
    EXPECT_EQ(208, test1.getDerivative());
    constexpr Polynomial test2("8x^9 + 10x^3 + 123", 2, 1, 3);
    EXPECT_EQ(18552, test2.getDerivative());
    constexpr Polynomial test3("x^5 + 19 + 3x^4 + 2x^3 + x^2 + x", 3, 1, 3);
    EXPECT_EQ(790, test3.getDerivative());
}


TEST(PolynomialTest, TestValue) {
    constexpr Polynomial test1("x^3 + 16x - 2", 8, 1, 3);
    EXPECT_EQ(638, test1.getValue());
    constexpr Polynomial test2("8x^9 + 10x^3 + 123", 2, 1, 3);
    EXPECT_EQ(4299, test2.getValue());
    constexpr Polynomial test3("x^5 + 19 + 3x^4 + 2x^3 + x^2 + x", 3, 1, 3);
    EXPECT_EQ(571, test3.getValue());
}

int main() {
    constexpr Polynomial test1("x^3 + 16x - 2", 8, 1, 3);
    static_assert(test1.getValue() == 638);
    return RUN_ALL_TESTS();
}