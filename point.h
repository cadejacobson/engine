#include "iostream"

#ifndef ENGINE_POINT_H
#define ENGINE_POINT_H

class Point {
public:
    double x, y;

    Point(double x1, double y1){
        x = x1;
        y = y1;
    }

    bool operator==(const Point& other) const {
        // Comparing each coordinate with a tolerance for floating-point comparison
        const double tolerance = 1e-9;
        std::cout << "== " <<((std::abs(x - other.x) < tolerance) && (std::abs(y - other.y) < tolerance)) << std::endl;
        return (std::abs(x - other.x) < tolerance) && (std::abs(y - other.y) < tolerance);
    }

    bool operator!=(const Point& other) const {
        const double tolerance = 1e-9;
        return !((std::abs(x - other.x) < tolerance) && (std::abs(y - other.y) < tolerance));
    }
};

#endif
