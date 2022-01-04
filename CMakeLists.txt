cmake_minimum_required(VERSION 3.19)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw2 main.cpp HashTable.h unionFind.h HashTable.cpp exceptions.h LinkedList.cpp LinkedList.h listTest.cpp
        randomListTest.cpp Game.cpp Game.h)
add_executable(hashTest randomHashTest.cpp HashTable.h HashTable.cpp exceptions.h LinkedList.cpp LinkedList.h)
add_executable(listTest exceptions.h LinkedList.cpp LinkedList.h randomListTest.cpp)
add_executable(RankTreeTest exceptions.h rankTree.h randomRankTest.cpp)
add_executable(UnionTest exceptions.h unionFind.h unionFindTest.cpp)