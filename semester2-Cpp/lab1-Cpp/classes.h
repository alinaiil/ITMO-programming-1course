#include <vector>
#include <cmath>

class Point {
private:
    double Ox;
    double Oy;
public:
    Point();

    Point(double x, double y);

    Point(const Point &point);

    double getX() const;

    double getY() const;

    bool operator==(const Point &point) const;

    Point &operator=(const Point &point);
};

class Polyline {
protected:
    std::vector<Point> points;
    int nPoints;
public:
    Polyline();

    Polyline(int n, Point *points);

    Polyline(const Polyline &polyline);

    int getNPoints() const;

    std::vector<Point> getPoints() const;

    Point getNthPoint(int n) const;

    double dist(int from, int to) const;

    virtual double countPerimeter() const;

    Polyline &operator=(const Polyline &polyline);
};

class ClosedPolyline : public Polyline {
public:
    explicit ClosedPolyline();

    explicit ClosedPolyline(int n, Point *points);

    ClosedPolyline(const ClosedPolyline &closedPolyline);

    double countPerimeter() const override;

    ClosedPolyline &operator=(const ClosedPolyline &closedPolyline);
};

class Polygon : public ClosedPolyline {
public:
    explicit Polygon();

    explicit Polygon(int n, Point *points);

    Polygon(const Polygon &polygon);

    virtual double countArea() const;

    Polygon &operator=(const Polygon &polygon);
};

class Triangle : public Polygon {
public:
    explicit Triangle();

    explicit Triangle(Point *points);

    Triangle(const Triangle &triangle);

    double countArea() const override;

    Triangle &operator=(const Triangle &triangle);
};

class Trapezoid : public Polygon {
public:
    explicit Trapezoid();

    explicit Trapezoid(Point *points);

    Trapezoid(const Trapezoid &trapezoid);

    double countArea() const override;

    Trapezoid &operator=(const Trapezoid &trapezoid);
};

class RegularPolygon : public Polygon {
public:
    explicit RegularPolygon();

    explicit RegularPolygon(int n, Point *points);

    RegularPolygon(const RegularPolygon &regularPolygon);

    double countArea() const override;

    RegularPolygon &operator=(const RegularPolygon &regularPolygon);
};