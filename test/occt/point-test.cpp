/*
    SPDX-License-Identifier: AGPL-3.0-or-later
    Copyright (C) 2025 Erick Ahmed
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <TopoDS_Shape.hxx>
#include "point.hpp"

TEST_CASE("Create points and get coordinates") {
    SECTION("Positive integer coordinates") {
        double coords[3] = {3, 7, 2};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        INFO("Point coordinates: " << x << ", " << y << ", " << z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Negative integer coordinates") {
        double coords[3] = {-2, -5, -14};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Mixed integer coordinates") {
        double coords[3] = {8, -5, 4};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Positive float coordinates") {
        double coords[3] = {3.2352, 7.124662, 2.5};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Negative float coordinates") {
        double coords[3] = {-2.1345, -5.463, -14.00001};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Mixed float coordinates") {
        double coords[3] = {8.124, -5.523, 4.0};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Origin") {
        double coords[3] = {0, 0, 0};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }

    SECTION("Large values") {
        double coords[3] = {65186415.16516, 948156654.0, -6516515.165};
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        double x, y, z;
        coord_point(p, &x, &y, &z);

        REQUIRE(x == Catch::Approx(coords[0]));
        REQUIRE(y == Catch::Approx(coords[1]));
        REQUIRE(z == Catch::Approx(coords[2]));
    }
}
