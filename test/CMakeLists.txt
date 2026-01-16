# Top-level Catch2 tests aggregator
cmake_minimum_required(VERSION 3.21)
project(CodeWizardTests)

# Add each module's tests
add_subdirectory(Bootstrap)
add_subdirectory(Platform)
add_subdirectory(Services)
add_subdirectory(Project)
add_subdirectory(Editor)
add_subdirectory(LanguageIntelligence)
add_subdirectory(BuildRun)
add_subdirectory(Persistence)
add_subdirectory(Extensibility)
add_subdirectory(UI)
