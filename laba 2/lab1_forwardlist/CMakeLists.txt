cmake_minimum_required(VERSION 3.10)
project(ForwardListLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/include)


add_subdirectory(src/lab1)

message(STATUS "Project configured successfully")