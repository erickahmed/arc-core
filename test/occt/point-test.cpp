/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <TopoDS_Shape.hxx>
#include "point.hpp"

TEST_CASE("make_point and coord_point behavior", "[make_point]") {
    auto check_coords = [](const TopoDS_Shape& p, const double expected[3]) {
        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE(x == Catch::Approx(expected[0]));
        REQUIRE(y == Catch::Approx(expected[1]));
        REQUIRE(z == Catch::Approx(expected[2]));
    };

    SECTION("Accept integer coordinates") {
        double coords[3] = {3, -7, 2};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
    }

    SECTION("Accept float coordinates") {
        double coords[3] = {3.2352, 7.124662, -2.5};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
    }

    SECTION("XYZ Origin") {
        double coords[3] = {0, 0, 0};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
    }

    SECTION("Large magnitude values") {
        double coords[3] = {6.5186415e7, 9.48156654e8, -6.516515e6};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
    }

    SECTION("Multiple points") {
        const double points[5][3] = {
            {3, 7, 2},
            {3, 7, 2},
            {-5, 3.7, 8},
            {-3, -7, -2},
            {0, 0, 1}
        };

        for (const auto& c : points) {
            TopoDS_Shape p = make_point(c[0], c[1], c[2]);
            check_coords(p, c);
        }
    }

    SECTION("Extreme double limits") {
        double coords[3] = {std::numeric_limits<double>::max(),
                            std::numeric_limits<double>::lowest(),
                            std::numeric_limits<double>::min()};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
    }

    SECTION("NaN and Infinity") {
        double coords[3] = {std::numeric_limits<double>::quiet_NaN(),
                            std::numeric_limits<double>::infinity(),
                            -std::numeric_limits<double>::infinity()};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);
        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE((std::isnan(x) || std::isinf(x)));
        REQUIRE((std::isnan(y) || std::isinf(y)));
        REQUIRE((std::isnan(z) || std::isinf(z)));
    }
}
