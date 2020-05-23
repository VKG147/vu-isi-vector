cmake_minimum_required(VERSION 3.15)
project(vector_class)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(vector_class
        average.cpp
        average.h
        main.cpp
        person.cpp
        person.h
        randomGenerator.cpp
        randomGenerator.h
        student.cpp
        student.h
        studentCompute.cpp
        studentCompute.h
        studentIO.cpp
        studentIO.h)
