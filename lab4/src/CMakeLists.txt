cmake_minimum_required(VERSION 3.22)
project(lab4)

set(CMAKE_C_STANDARD 99)

add_library(lib1_s STATIC first.c lib.h)
add_library(lib2_s STATIC second.c lib.h)
add_executable(prog1_s static_main.c)
add_executable(prog2_s static_main.c)
target_link_libraries(prog1_s lib1_s)
target_link_libraries(prog2_s lib2_s)

add_library(lib1_d SHARED first.c lib.h)
add_library(lib2_d SHARED second.c lib.h)
add_executable(prog_d dynamic_main.c)
target_link_libraries(lib1_d)
target_link_libraries(lib2_d)
target_link_libraries(prog_d ${CMAKE_DL_LIBS})
