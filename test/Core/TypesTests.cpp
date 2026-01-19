#include <catch2/catch_test_macros.hpp>
#include <Core/Types.h>

using namespace CodeWizard::Core;

TEST_CASE("Position comparison", "[core][types]") {
    Position p1{0, 0};
    Position p2{0, 1};
    Position p3{1, 0};

    REQUIRE(p1 < p2);
    REQUIRE(p1 < p3);
    REQUIRE(p2 < p3);
    REQUIRE(p1 == p1);
}

TEST_CASE("TextRange validity", "[core][types]") {
    TextRange valid{{0,0}, {1,0}};
    TextRange invalid{{1,0}, {0,0}};

    REQUIRE(valid.valid());
    REQUIRE_FALSE(invalid.valid());
}
