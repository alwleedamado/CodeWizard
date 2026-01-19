#include <catch2/catch_test_macros.hpp>
#include <Core/Result.h>

using namespace CodeWizard::Core;

TEST_CASE("Result handles success values", "[core][result]") {
    Result<int> result = success(42);
    REQUIRE(result.hasValue());
    REQUIRE(result.value() == 42);
}

TEST_CASE("Result handles errors", "[core][result]") {
    Result<int> result = failure<int>(ErrorCode::FileNotFound, "File missing");
    REQUIRE(result.hasError());
    REQUIRE(result.error().code() == ErrorCode::FileNotFound);
    REQUIRE(result.error().message() == "File missing");
}

TEST_CASE("Result<void> works", "[core][result]") {
    Result<void> ok = success();
    REQUIRE(ok.hasValue());

    Result<void> err = failure(ErrorCode::IoError, "Disk full");
    REQUIRE(err.hasError());
}
