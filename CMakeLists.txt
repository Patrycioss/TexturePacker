cmake_minimum_required(VERSION 3.30)
project(texture_packer)


add_subdirectory(ext)

set(CMAKE_CXX_STANDARD 20)

add_executable(texture_packer main.cpp)
target_link_libraries(texture_packer PRIVATE dependencies)
