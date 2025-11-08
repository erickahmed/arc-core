#pragma once

#include <gp_Pnt.hxx>

extern "C" {
    struct Point {
        double x;
        double y;
        double z;
    };

Point* make_point(double x, double y, double z);
void free_point(Point* p);
void coord_point(const Point* p, double* x, double* y, double* z);
}
