#include "boundary.h"
#include "point.h"
#include <cstdlib>
#include <ctime>
#include <vector>

#ifndef ENGINE_RAY_H
#define ENGINE_RAY_H

class Ray {
public:
    double x;
    double y;
    double dir;

    Ray(double x1, double y1, double dir1){
        x = x1;
        y = y1;
        dir = dir1;
    }

    Point cast(Boundary &boundary) {
        const int x1 = boundary.x1;
        const int y1 = boundary.y1;
        const int x2 = boundary.x2;
        const int y2 = boundary.y2;

        const double x3 = x;
        const double y3 = y;
        const double x4 = x + cos(dir);
        const double y4 = y + sin(dir);

        const double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (den < .000000001) {
            return {INT16_MAX, INT16_MAX};
        }

        const double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        const double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
        if (t > 0 && t < 1 && u > 0) {
            Point pt = Point(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
           //std::cout << "point: " << pt.x << " " << pt.y << std::endl;
            return pt;
        }
        return {INT16_MAX, INT16_MAX};
    }
};

#endif
