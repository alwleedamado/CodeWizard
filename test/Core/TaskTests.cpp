#include <catch2/catch_test_macros.hpp>
#include <Core/Task.h>
#include <thread>
#include <chrono>

using namespace CodeWizard::Core;

TEST_CASE("TaskScheduler executes tasks", "[core][task]") {
    bool executed = false;
    auto& scheduler = TaskScheduler::instance();

    auto id = scheduler.schedule([&executed]() {
        executed = true;
    }, std::chrono::milliseconds(10));

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    REQUIRE(executed);
}

TEST_CASE("TaskScheduler cancels tasks", "[core][task]") {
    bool executed = false;
    auto& scheduler = TaskScheduler::instance();

    auto id = scheduler.schedule([&executed]() {
        executed = true;
    }, std::chrono::milliseconds(100));

    scheduler.cancel(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    REQUIRE_FALSE(executed);
}
