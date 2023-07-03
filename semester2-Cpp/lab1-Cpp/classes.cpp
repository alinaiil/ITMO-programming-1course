#include "classes.h"
#include <stdexcept>

Point::Point() {
    Ox = 0;
    Oy = 0;
}

Point::Point(double x, double y) {
    Ox = x;
    Oy = y;
}

Point::Point(const Point &point) {
    Ox = point.Ox;
    Oy = point.Oy;
}

double Point::getX() const {
    return Ox;
}

double Point::getY() const {
    return Oy;
}

bool Point::operator==(const Point &point) const {
    return (point.getX() == this->getX() && point.getY() == this->getY());
}

Point &Point::operator=(const Point &point) {
    Ox = point.getX();
    Oy = point.getY();
    return *this;
}

Polyline::Polyline() {
    nPoints = 0;
}

Polyline::Polyline(int n, Point *points) {
    nPoints = n;
    for (int i = 0; i < n; i++) {
        this->points.push_back(points[i]);
    }
}

Polyline::Polyline(const Polyline &polyline) {
    nPoints = polyline.getNPoints();
    points = polyline.getPoints();
}

int Polyline::getNPoints() const {
    return nPoints;
}

Point Polyline::getNthPoint(int n) const {
    return points[n];
}

double Polyline::dist(int from, int to) const {
    return sqrt(pow(getNthPoint(from).getX() - getNthPoint(to).getX(), 2) +
                pow(getNthPoint(from).getY() - getNthPoint(to).getY(), 2));
}

double Polyline::countPerimeter() const {
    double length = 0;
    for (int i = 1; i < nPoints; i++) {
        length += dist(i - 1, i);
    }
    return length;
}

Polyline &Polyline::operator=(const Polyline &polyline) {
    nPoints = polyline.getNPoints();
    points = polyline.getPoints();
    return *this;
}

std::vector<Point> Polyline::getPoints() const {
    return points;
}

ClosedPolyline::ClosedPolyline() : Polyline() {}

ClosedPolyline::ClosedPolyline(int n, Point *points) : Polyline(n, points) {}

ClosedPolyline::ClosedPolyline(const ClosedPolyline &closedPolyline) = default;

double ClosedPolyline::countPerimeter() const {
    double cycleLength = dist(getNPoints() - 1, 0);
    return cycleLength + Polyline::countPerimeter();
}

ClosedPolyline &ClosedPolyline::operator=(const ClosedPolyline &closedPolyline) {
    nPoints = closedPolyline.getNPoints();
    points = closedPolyline.getPoints();
    return *this;
}

bool checkPoints(const Polygon &polygon) {
    int n = polygon.getNPoints();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            if (polygon.getNthPoint(i) == polygon.getNthPoint(j)) {
                return true;
            }
        }
    }
    return false;
}

Polygon::Polygon() : ClosedPolyline() {}

Polygon::Polygon(int n, Point *points) : ClosedPolyline(n, points) {
    if (checkPoints(*this)) {
        throw std::logic_error("Invalid polygon");
    }
}

Polygon::Polygon(const Polygon &polygon) = default;

double Polygon::countArea() const {
    double area = 0;
    for (int i = 0; i < getNPoints() - 1; i++) {
        area += getNthPoint(i).getX() * getNthPoint(i + 1).getY();
        area -= getNthPoint(i + 1).getX() * getNthPoint(i).getY();
    }
    area += getNthPoint(getNPoints() - 1).getX() * getNthPoint(0).getY();
    area -= getNthPoint(0).getX() * getNthPoint(getNPoints() - 1).getY();
    area = fabs(area) / 2;
    return area;
}

Polygon &Polygon::operator=(const Polygon &polygon) {
    nPoints = polygon.getNPoints();
    points = polygon.getPoints();
    return *this;
}

Triangle::Triangle() : Polygon() {}

Triangle::Triangle(Point *points) : Polygon(3, points) {
    double a = dist(0, 1);
    double b = dist(1, 2);
    double c = dist(2, 0);
    if ((a + b <= c) || (b + c <= a) || (c + a <= b)) {
        throw std::logic_error("The shape is not triangle.");
    }
    if (checkPoints(*this)) {
        throw std::logic_error("The shape is not triangle.");
    }
}

Triangle::Triangle(const Triangle &triangle) = default;

double Triangle::countArea() const {
    double p = (dist(0, 1) + dist(1, 2) + dist(2, 0)) / 2;
    double area = sqrt(p * (p - dist(0, 1)) * (p - dist(1, 2)) * (p - (dist(2, 0))));
    return area;
}

Triangle &Triangle::operator=(const Triangle &triangle) {
    nPoints = triangle.getNPoints();
    points = triangle.getPoints();
    return *this;
}

Trapezoid::Trapezoid() : Polygon() {}

Trapezoid::Trapezoid(Point *points) : Polygon(4, points) {
    if (fabs(getNthPoint(0).getY() - getNthPoint(1).getY()) == 0 ||
        fabs(getNthPoint(2).getY() - getNthPoint(3).getY()) == 0) {
        if (fabs(getNthPoint(0).getY() - getNthPoint(1).getY()) != 0 ||
        fabs(getNthPoint(2).getY() - getNthPoint(3).getY()) != 0) {
            throw std::logic_error("The shape is not trapezoid.");
        }
    } else if (fabs(getNthPoint(1).getY() - getNthPoint(2).getY()) == 0 ||
               fabs(getNthPoint(3).getY() - getNthPoint(0).getY()) == 0) {
        if (fabs(getNthPoint(1).getY() - getNthPoint(2).getY()) != 0 ||
            fabs(getNthPoint(3).getY() - getNthPoint(0).getY()) != 0) {
            throw std::logic_error("The shape is not trapezoid.");
        }
    } else {
        double k1 = fabs(getNthPoint(0).getX() - getNthPoint(1).getX()) /
                fabs(getNthPoint(0).getY() - getNthPoint(1).getY());
        double k2 = fabs(getNthPoint(1).getX() - getNthPoint(2).getX()) /
                fabs(getNthPoint(1).getY() - getNthPoint(2).getY());
        double k3 = fabs(getNthPoint(2).getX() - getNthPoint(3).getX()) /
                fabs(getNthPoint(2).getY() - getNthPoint(3).getY());
        double k4 = fabs(getNthPoint(3).getX() - getNthPoint(0).getX()) /
                fabs(getNthPoint(0).getY() - getNthPoint(3).getY());
        if (k1 != k3 && k2 != k4) {
            throw std::logic_error("The shape is not trapezoid.");
        }
    }
    if (checkPoints(*this)) {
        throw std::logic_error("The shape is not trapezoid.");
    }
}

Trapezoid::Trapezoid(const Trapezoid &trapezoid) = default;

double Trapezoid::countArea() const {
    double k1 = fabs(getNthPoint(0).getX() - getNthPoint(1).getX()) /
                fabs(getNthPoint(0).getY() - getNthPoint(1).getY());
    double k3 = fabs(getNthPoint(2).getX() - getNthPoint(3).getX()) /
                fabs(getNthPoint(2).getY() - getNthPoint(3).getY());
    double a, b, c, d;
    if (k1 == k3) {
        a = dist(0, 1);
        b = dist(2, 3);
        c = dist(1, 2);
        d = dist(3, 0);
    } else {
        a = dist(3, 0);
        b = dist(1, 2);
        c = dist(0, 1);
        d = dist(2, 3);
    }
    double h = sqrt(pow(c, 2) - pow(((pow((a - b), 2) + pow(c, 2) - pow(d, 2)) / (2 * (a - b))), 2));
    double area = ((a + b) * h) / 2;
    return area;
}

Trapezoid &Trapezoid::operator=(const Trapezoid &trapezoid) {
    nPoints = trapezoid.getNPoints();
    points = trapezoid.getPoints();
    return *this;
}

RegularPolygon::RegularPolygon() : Polygon() {}

RegularPolygon::RegularPolygon(int n, Point *points) : Polygon(n, points) {
    double a = dist(0, 1);
    bool flag = false;
    for (int i = 0; i < n - 1; i++) {
        if (a != dist(i, i + 1)) {
            flag = true;
        }
    }
    if (a != dist(n - 1, 0)) {
        flag = true;
    }
    if (checkPoints(*this)) {
        flag = true;
    }
    if (n < 3) {
        flag = true;
    }
    if (flag) {
        throw std::logic_error("The shape is not regular polygon.");
    }
}

RegularPolygon::RegularPolygon(const RegularPolygon &regularPolygon) = default;

double RegularPolygon::countArea() const {
    double a = dist(0, 1);
    return ((getNPoints() * pow(a, 2)) / (4 * tan(M_PI / getNPoints())));
}

RegularPolygon &RegularPolygon::operator=(const RegularPolygon &regularPolygon) {
    nPoints = regularPolygon.getNPoints();
    points = regularPolygon.getPoints();
    return *this;
}