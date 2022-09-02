cmake_minimum_required(VERSION 3.10.2)
project(time_wheel 
        VERSION 1.0
        DESCRIPTION "Timewheel Implementation Example in C"
        LANGUAGES C)

add_compile_options(-Wall)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

add_executable(twtest main.c time_wheel.c)