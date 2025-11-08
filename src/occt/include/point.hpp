#pragma once

#include <gp_Pnt.hxx>

extern "C" {
    TopoDS_Shape make_point(double x, double y, double z);
    void coord_point(const TopoDS_Shape& shape, double* x, double* y, double* z);
}
