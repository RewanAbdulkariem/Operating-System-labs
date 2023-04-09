cmake_minimum_required(VERSION 3.25)
project(lab_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab_3 caltrain-runner.c caltrain.c)
target_link_libraries(lab_3 -pthread)
