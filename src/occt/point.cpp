/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Pnt.hxx>

struct point_shape_t {
    TopoDS_Shape shape;
};

extern "C" {
    point_shape_t* make_point(double x, double y, double z) {
        point_shape_t* result = new point_shape_t();
        gp_Pnt point(x, y, z);
        result->shape = BRepBuilderAPI_MakeVertex(point).Vertex();
        return result;
    }

    void coord_point(const point_shape_t* shape, double* x, double* y, double* z) {
        if (!shape || !x || !y || !z) return;

        TopoDS_Vertex vertex = TopoDS::Vertex(shape->shape);
        gp_Pnt point = BRep_Tool::Pnt(vertex);
        *x = point.X();
        *y = point.Y();
        *z = point.Z();
    }

    void delete_point(point_shape_t* shape) {
        delete shape;
    }
}
