project(dependencies)


add_subdirectory(glfw)


add_library(dependencies STATIC

        glad/src/glad.c

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(dependencies PUBLIC
        glfw)

target_include_directories(dependencies PUBLIC
        glad/include
        glfw/include
        imgui
)