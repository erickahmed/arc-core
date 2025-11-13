/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_approx.hpp>
#include "point.hpp"

TEST_CASE("Test point creation, query and deletion behaviour", "[point]") {
    auto check_coords = [](const point_shape_t* p, const double expected[3]) {
        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE(x == Catch::Approx(expected[0]));
        REQUIRE(y == Catch::Approx(expected[1]));
        REQUIRE(z == Catch::Approx(expected[2]));
    };

    SECTION("Accept integer coordinates") {
        double coords[3] = {3, -7, 2};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("Accept float coordinates") {
        double coords[3] = {3.2352, 7.124662, -2.5};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("XYZ Origin") {
        double coords[3] = {0, 0, 0};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("Large magnitude values") {
        double coords[3] = {6.5186415e7, 9.48156654e8, -6.516515e6};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
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
            point_shape_t* p = make_point(c[0], c[1], c[2]);
            check_coords(p, c);
            delete_point(p);
        }
    }

    SECTION("Extreme double limits") {
        double coords[3] = {std::numeric_limits<double>::max(),
                            std::numeric_limits<double>::lowest(),
                            std::numeric_limits<double>::min()};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("NaN and Infinity") {
        double coords[3] = {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
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

    SECTION("Denormal values") {
        double coords[3] = {std::numeric_limits<double>::denorm_min(),
                            -std::numeric_limits<double>::denorm_min(),
                            0.0};
        point_shape_t* p = make_point(coords[0], coords[1], coords[2]);
        check_coords(p, coords);
        delete_point(p);
    }

    SECTION("Clear single point") {
        point_shape_t* p = make_point(1.0, 2.0, 3.0);

        double x, y, z;
        coord_point(p, &x, &y, &z);
        REQUIRE(x == Catch::Approx(1.0));
        REQUIRE(y == Catch::Approx(2.0));
        REQUIRE(z == Catch::Approx(3.0));

        delete_point(p);
    }

    SECTION("Clear multiple points") {
        const int num_points = 5;
        point_shape_t* p[num_points];

        for (int i = 0; i < num_points; ++i) {
            p[i] = make_point(i * 1.0, i * 2.0, i * 3.0);
        }

        for (int i = 0; i < num_points; ++i) {
            delete_point(p[i]);
        }
    }

    SECTION("Clear already null shape") {
        point_shape_t* null_p = nullptr;
        REQUIRE(null_p == nullptr);
        delete_point(null_p);
    }

    SECTION("Memory efficiency - multiple clear calls") {
        for (int i = 0; i < 100; ++i) {
            point_shape_t* tmp_p = make_point(1.0, 1.0, 1.0);
            delete_point(tmp_p);
        }
    }

    SECTION("Pointer validity after operations") {
        point_shape_t* p = make_point(1.0, 2.0, 3.0);
        REQUIRE(p != nullptr);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        delete_point(p);
    }
}

TEST_CASE("Benchmark point operations") {
    SECTION("make_point performance") {
        point_shape_t* p = nullptr;
        BENCHMARK("Create point") {
            return p = make_point(1.0, 2.0, 3.0);
        };
        delete_point(p);
    }

    SECTION("coord_point performance") {
        point_shape_t* p = make_point(1.0, 2.0, 3.0);
        double x, y, z;

        BENCHMARK("Extract coordinates") {
            coord_point(p, &x, &y, &z);
        };
        delete_point(p);
    }

    SECTION("delete_point performance") {
        point_shape_t* p = make_point(1.0, 2.0, 3.0);
        BENCHMARK("Delete point") {
            delete_point(p);
            return p;
        };
    }
}
