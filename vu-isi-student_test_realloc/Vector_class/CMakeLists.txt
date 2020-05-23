cmake_minimum_required(VERSION 3.15)
project(vector_class)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

include_directories(vu-isi-vector)

add_executable(vector_class
        average.cpp
        average.h
        main.cpp
        person.h
        randomGenerator.cpp
        randomGenerator.h
        student.cpp
        student.h
        studentCompute.cpp
        studentCompute.h
        studentIO.cpp
        studentIO.h)

target_link_libraries(vector_class PRIVATE "-Wl,--stack,10000000")
