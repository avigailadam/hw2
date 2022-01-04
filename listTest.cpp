//
// Created by avi on 03/01/2022.
//
#include <iostream>
#include "cassert"
#include "LinkedList.h"

int main() {
    LinkedList list;
    list.insert(1, 2,0);
    assert(list.getSize() == 1);
    list.insert(3, 8,9);
    assert(list.getSize() == 2);
    list.insert(2, 3,10);
    assert(list.getSize() == 3);
    list.insert(5, 9,3);
    assert(list.getSize() == 4);
    list.remove(5);
    assert(list.getSize() == 3);

}