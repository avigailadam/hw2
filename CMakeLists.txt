cmake_minimum_required(VERSION 3.19)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw2 main2.cpp library2.cpp library2.h RankTree.h HashTable.h UnionFind.h HashTable.cpp exceptions.h LinkedList.cpp
        LinkedList.h my_vector.h Game.cpp Game.h Group.h GroupInfoByScore.h PlayerByID.h)
add_executable(hashTest randomHashTest.cpp HashTable.h HashTable.cpp exceptions.h LinkedList.cpp LinkedList.h)
add_executable(listTest exceptions.h LinkedList.cpp LinkedList.h randomListTest.cpp)
add_executable(RankTreeTest exceptions.h RankTree.h randomRankTest.cpp)
add_executable(UnionTest exceptions.h UnionFind.h unionFindTest.cpp)