/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#pragma once

#include <gp_Pnt.hxx>

TopoDS_Shape make_point(double x, double y, double z);
void coord_point(const TopoDS_Shape& shape, double* x, double* y, double* z);
