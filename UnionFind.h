//
// Created by hagai on 1/3/2022.
//

#ifndef HW1_UNIONFIND_H
#define HW1_UNIONFIND_H

#include <iostream>
#include "exceptions.h"
#include "Group.h"

#define EMPTY -1

class UnionFind {
    Group **groups;
    int *parent;
    int numOfGroups;


    void updateFather(int i, int father) {
        if (parent[i] == father || parent[i] == EMPTY)
            return;
        parent[i] = father;
        updateFather(parent[i], father);
    }

    int internal_find(int i) {
        return parent[i] == EMPTY ? i : internal_find(parent[i]);
    }
public:

    UnionFind(int k, int scale) : numOfGroups(k + 1) {
        groups = new Group *[numOfGroups];
        parent = new int[numOfGroups];
        if (!groups || !parent) {
            delete[] groups;
            delete[] parent;
            throw std::bad_alloc();
        }
        for (int i = 0; i <= numOfGroups; ++i) {
            parent[i] = EMPTY;
            groups[i] = new Group(scale);
        }
    }

    ~UnionFind() {
        for (int i = 0; i <= numOfGroups; ++i) {
            delete groups[i];
        }
        delete[] groups;
        delete[] parent;
    }


    void Union(int A, int B) {
        int groupA = find(A);
        int groupB = find(B);
        if (groupA == groupB)
            return;
        if (groups[groupA]->getSize() < groups[groupB]->getSize()) {
            parent[groupA] = groupB;
            groups[groupB]->mergeGroups(groups[groupA]);
        } else {
            parent[groupB] = groupA;
            groups[groupA]->mergeGroups(groups[groupB]);
        }

    }

    int find(int i) {
        int father = internal_find(i);
        updateFather(i, father);
        return father;
    }

    Group* getGroup(int i){
        return groups[find(i)];
    }

    friend std::ostream &operator<<(std::ostream &os, const UnionFind &current) {
        for (int i = 0; i <= current.numOfGroups; ++i) {
            os << i << " is son of " << current.parent[i] << " his size is: " << current.groups[i]->getSize()
               << std::endl;
        }
        os << std::endl;
        return os;
    }
};


#endif //HW1_UNIONFIND_H
