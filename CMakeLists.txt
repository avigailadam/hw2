cmake_minimum_required(VERSION 3.19)
project(hw2)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw2 main.cpp HashTable.h HashTable.cpp exceptions.h LinkedList.cpp LinkedList.h listTest.cpp randomListTest.cpp)
add_executable(hashTest randomHashTest.cpp HashTable.h HashTable.cpp exceptions.h LinkedList.cpp LinkedList.h)
add_executable(listTest exceptions.h LinkedList.cpp LinkedList.h randomListTest.cpp)