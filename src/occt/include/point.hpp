/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#ifndef POINT_HPP
#define POINT_HPP

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct point_shape point_shape_t;

point_shape_t* make_point(double x, double y, double z);
void coord_point(const point_shape_t* shape, double* x, double* y, double* z);
void delete_point(point_shape_t* shape);

#ifdef __cplusplus
}
#endif

#endif
