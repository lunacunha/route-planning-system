cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled
        main.cpp
        src/Graph.h
        src/GraphInterface.cpp
        src/Location.cpp
        src/Location.h
        src/Menu.h
        src/Menu.cpp
        src/MutablePriorityQueue.h
)