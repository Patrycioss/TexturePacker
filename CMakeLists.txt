cmake_minimum_required(VERSION 3.30)
project(texture_packer)


add_subdirectory(ext)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(texture_packer ${SOURCES} ${HEADERS})


target_include_directories(texture_packer PUBLIC include)
target_link_libraries(texture_packer PRIVATE dependencies)
target_compile_definitions(texture_packer PRIVATE RESOURCES="${CMAKE_CURRENT_BINARY_DIR}/resources")
