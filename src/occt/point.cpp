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

TopoDS_Shape make_point(double x, double y, double z) {
    gp_Pnt point(x, y, z);
    return BRepBuilderAPI_MakeVertex(point).Vertex();
}

void coord_point(const TopoDS_Shape& shape, double* x, double* y, double* z) {
    TopoDS_Vertex vertex = TopoDS::Vertex(shape);
    gp_Pnt point = BRep_Tool::Pnt(vertex);

    *x = point.X();
    *y = point.Y();
    *z = point.Z();
}
