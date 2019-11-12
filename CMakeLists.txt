cmake_minimum_required(VERSION 3.10)
project(spell_checker_extra_credit C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(spell_checker_extra_credit main.c)
target_link_libraries(${PROJECT_NAME} pthread)
