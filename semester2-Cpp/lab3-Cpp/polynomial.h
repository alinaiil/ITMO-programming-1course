#include "sstream"
#include <map>

class Polynomial {
private:
    std::map<int, double> coefficients;
    int n;
    int maxDegree;
public:
    explicit Polynomial();

    explicit Polynomial(int size, const double *givenCoefficients);

    explicit Polynomial(int size, const std::map<int, double>& givenCoefficients);

    Polynomial(const Polynomial &polynomial);

    ~Polynomial();

    int getN() const;

    std::map<int, double> getCoefficients();

    Polynomial &operator=(const Polynomial &polynomial);

    bool operator==(const Polynomial &polynomial);

    bool operator!=(const Polynomial &polynomial);

    Polynomial operator+(const Polynomial &polynomial);

    Polynomial operator-() const;

    Polynomial operator-(const Polynomial &polynomial);

    Polynomial operator+=(const Polynomial &polynomial);

    Polynomial operator-=(const Polynomial &polynomial);

    Polynomial operator*(const Polynomial &polynomial);

    Polynomial operator*(double number);

    Polynomial operator/(double number);

    Polynomial operator*=(const Polynomial &polynomial);

    Polynomial operator*=(double number);

    Polynomial operator/=(double number);

    friend std::ostream& operator<<(std::ostream &out, const Polynomial &polynomial);

    friend std::istream& operator>>(std::istream &in, Polynomial &polynomial);

    double operator[](int degree);
};