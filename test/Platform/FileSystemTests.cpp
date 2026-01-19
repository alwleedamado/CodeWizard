#include <catch2/catch_test_macros.hpp>
#include <Platform/FileSystem.h>
#include <Platform/Path.h>
#include <fstream>

using namespace CodeWizard::Platform;

TEST_CASE("File read/write", "[platform][filesystem]") {
    Path tempFile("/tmp/codewizard_test.txt");

    // Write
    auto writeResult = writeFile(tempFile, "Hello, CodeWizard!");
    REQUIRE(writeResult.hasValue());

    // Read
    auto readResult = readFile(tempFile);
    REQUIRE(readResult.hasValue());
    REQUIRE(readResult.value() == "Hello, CodeWizard!");

    // Cleanup
    remove(tempFile);
}

TEST_CASE("Directory listing", "[platform][filesystem]") {
    Path tempDir("/tmp/cw_test_dir");
    auto created = createDirectory(tempDir);

    Path file1 = tempDir / "file1.txt";
    Path file2 = tempDir / "file2.cpp";
    writeFile(file1, "test");
    writeFile(file2, "code");

    auto listResult = listDirectory(tempDir);
    REQUIRE(listResult.hasValue());
    REQUIRE(listResult.value().size() >= 2);

    remove(tempDir, true);
}
