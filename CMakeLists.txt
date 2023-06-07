cmake_minimum_required(VERSION 3.21)
project(DataStructuresHW1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DataStructuresHW1
        AVLMultiVariable.h
        AVLTree.h
        ComplexNode.h
        Exception.h
        GenericNode.h
        Group.cpp
        Group.h

        maina1.cpp
        Movie.cpp
        Movie.h
        Node.h
        StreamingDBa1.cpp
        StreamingDBa1.h
        User.cpp
        User.h
        wet1util.h)
