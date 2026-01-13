cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# ---------- self-contained sources ----------
qt_add_executable(codewizard
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/ui/MainWindow.ui
        src/EditorWidget.cpp
        src/EditorWidget.h
        # QTextBrowser float
)

target_link_libraries(codewizard PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# Size-first release flags
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_options(codewizard PRIVATE -Oz -flto)
    target_link_options(codewizard PRIVATE -flto -s)
endif()

# Install next to build dir for quick test
install(TARGETS codewizard DESTINATION ${PROJECT_BINARY_DIR})

qt_finalize_executable(codewizard)