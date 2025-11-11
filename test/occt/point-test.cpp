/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>
#include <TopoDS_Shape.hxx>
#include "point.hpp"

TEST_CASE("Test point creation, query and deletion behaviour", "[point]") {
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

    SECTION("Clear single point") {
        TopoDS_Shape point = make_point(1.0, 2.0, 3.0);

        REQUIRE_FALSE(point.IsNull());
        double x, y, z;
        coord_point(point, &x, &y, &z);
        REQUIRE(x == Catch::Approx(1.0));
        REQUIRE(y == Catch::Approx(2.0));
        REQUIRE(z == Catch::Approx(3.0));

        clear_point(point);

        REQUIRE(point.IsNull());
    }

    SECTION("Clear multiple points") {
        const int num_points = 5;
        TopoDS_Shape points[num_points];

        for (int i = 0; i < num_points; ++i) {
            points[i] = make_point(i * 1.0, i * 2.0, i * 3.0);
            REQUIRE_FALSE(points[i].IsNull());
        }

        for (int i = 0; i < num_points; ++i) {
            clear_point(points[i]);
            REQUIRE(points[i].IsNull());
        }
    }

    SECTION("Clear already null shape") {
        TopoDS_Shape null_shape;
        REQUIRE(null_shape.IsNull());

        clear_point(null_shape);
        REQUIRE(null_shape.IsNull());
    }

    SECTION("Memory efficiency - multiple clear calls") {
        TopoDS_Shape point = make_point(1.0, 1.0, 1.0);

        for (int i = 0; i < 100; ++i) {
            clear_point(point);
            REQUIRE(point.IsNull());
        }
    }
}

TEST_CASE("Benchmark point operations") {
    SECTION("make_point performance") {
        BENCHMARK("create point") {
            return make_point(1.0, 2.0, 3.0);
        };
    }

    SECTION("coord_point performance") {
        TopoDS_Shape point = make_point(1.0, 2.0, 3.0);
        double x, y, z;

        BENCHMARK("extract coordinates") {
            coord_point(point, &x, &y, &z);
        };
    }

    SECTION("clear_point performance") {
        BENCHMARK("create and clear point") {
            TopoDS_Shape point = make_point(1.0, 2.0, 3.0);
            clear_point(point);
            return point.IsNull();
        };
    }
}
