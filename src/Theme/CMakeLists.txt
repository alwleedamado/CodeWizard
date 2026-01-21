qt6_wrap_cpp(UI_MOC_SRCS
    include/Theme/ThemeEngine.h
)
add_library(Theme STATIC ${UI_MOC_SRCS} src/ThemeEngine.cpp)

target_include_directories(Theme PUBLIC include)
target_link_libraries(Theme PUBLIC Qt6::Widgets)