cmake_minimum_required(VERSION 3.10)
project(StudentGrades CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimized (Release) build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(students ${SOURCES})
