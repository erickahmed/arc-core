/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "point.hpp"

TEST_CASE("Point lifecycle and coordinate operations", "[point]") {
    auto check_coords = [](const point_shape_t* p, const double expected[3]) {
        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE(x == Catch::Approx(expected[0]));
        REQUIRE(y == Catch::Approx(expected[1]));
        REQUIRE(z == Catch::Approx(expected[2]));
    };

    SECTION("Basic creation and coordinate retrieval") {
        double coords[3] = {3.2352, 7.124662, -2.5};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("Extreme double values") {
        double coords[3] = {
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::min()
        };
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("Special floating-point values") {
        double coords[3] = {
            std::numeric_limits<double>::quiet_NaN(),
            std::numeric_limits<double>::infinity(),
            -std::numeric_limits<double>::infinity()
        };
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE(std::isnan(x));
        REQUIRE(std::isinf(y));
        REQUIRE(std::isinf(z));
        REQUIRE(y > 0);
        REQUIRE(z < 0);

        delete_point(p);
    }

    SECTION("Multiple independent instances") {
        const double points[3][3] = {
            {3, 7, 2},
            {-5, 3.7, 8},
            {-3, -7, -2}
        };

        for (const auto& coords : points) {
            point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
            check_coords(p, coords);
            delete_point(p);
        }
    }

    SECTION("Null pointer safety") {
        point_shape_t* null_p = nullptr;
        delete_point(null_p);

        double x, y, z;
        coord_point(null_p, &x, &y, &z);
    }
}

TEST_CASE("Point memory management", "[point]") {
    SECTION("Rapid allocation/deallocation cycle") {
        for (int i = 0; i < 50; ++i) {
            point_shape_t* p = make_point(1.0, 2.0, 3.0);
            REQUIRE(p != nullptr);
            delete_point(p);
        }
    }
}
