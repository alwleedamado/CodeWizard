#include <catch2/catch_test_macros.hpp>
#include <Editor/TextBuffer.h>

using namespace CodeWizard::Editor;

TEST_CASE("TextBuffer line access", "[editor][textbuffer]") {
    TextBuffer buffer("line1\nline2\nline3");
    REQUIRE(buffer.lineCount() == 3);
    REQUIRE(buffer.line(0) == "line1");
    REQUIRE(buffer.line(1) == "line2");
    REQUIRE(buffer.line(2) == "line3");
}

TEST_CASE("TextBuffer position conversion", "[editor][textbuffer]") {
    TextBuffer buffer("hello\nworld");
    auto pos = buffer.positionFromOffset(7); // 'o' in "world"
    REQUIRE(pos.line == 1);
    REQUIRE(pos.character == 1);

    auto offset = buffer.offsetFromPosition({1, 1});
    REQUIRE(offset == 7);
}
