# src/UI/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES "src/*.cpp")
qt6_wrap_cpp(UI_MOC_SRCS
    include/UI/MainWindow.h
    include/UI/EditorTab.h
    include/UI/CommandBar.h
    include/UI/ProjectView.h
    include/UI/SidebarTabBar.h
    include/UI/Sidebar.h
    include/UI/StatusBar.h
    include/UI/UIApplication.h
)

add_library(UI STATIC ${SOURCES} ${UI_MOC_SRCS})

target_include_directories(UI PUBLIC include)
target_link_libraries(UI PUBLIC
    Core
    Platform
    Services
    Editor
    LanguageIntelligence
    Qt6::Widgets
    Theme
)

target_compile_features(UI PUBLIC cxx_std_20)
