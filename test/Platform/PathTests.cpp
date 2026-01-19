#include <catch2/catch_test_macros.hpp>
#include <Platform/Path.h>

using namespace CodeWizard::Platform;

TEST_CASE("Path construction and normalization", "[platform][path]") {
    Path p("/home/user//test/../file.txt");
    REQUIRE(p.native() == "/home/user/file.txt");
}

TEST_CASE("Path operators", "[platform][path]") {
    Path base("/home/user");
    Path file("document.txt");
    Path combined = base / file;
    REQUIRE(combined.native() == "/home/user/document.txt");
}
