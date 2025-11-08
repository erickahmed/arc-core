#include <catch2/catch_test_macros.hpp>
#include "point.hpp"

TEST_CASE("Create points") {
    SECTION("Positive integer coordinates") {
        int coords[3] = [3,7,2];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Negative integer coordinates") {
        int coords[3] = [-2,-5,-14];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Mixed integer coordinates") {
        int coords[3] = [8,-5,4];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Positive float coordinates") {
        double coords[3] = [3.2352,7.124662,2.5];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Negative float coordinates") {
        double coords[3] = [-2.1345,-5.463,-14.00001];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Mixed float coordinates") {
        double coords[3] = [8.124,-5.523,4.0000];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Origin float") {
        int coords[3] = [0,0,0];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }

    SECTION("Large values") {
        float coords[3] = [65186415.16516, 948156654, -6516515.165];
        TopoDS_Shape p = make_point(coords[0], coords[1], coords[2]);

        INFO("Point coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2]);

        REQUIRE(p.x == coords[0]);
        REQUIRE(p.y == coords[1]);
        REQUIRE(p.z == coords[2]);
    }
}

/*
TEST_CASE("Get points") {
    SECTION("Get point 1") {
    REQUIRE();
    }

    SECTION("Get point 2") {
    REQUIRE();
    }

    SECTION("Get point 3") {
    REQUIRE();
    }

    SECTION("Get point 4") {
    REQUIRE();
    }

    SECTION("Get point 5") {
    REQUIRE();
    }
}
*/
