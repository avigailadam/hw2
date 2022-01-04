//
// Created by hagai on 1/3/2022.
//

#ifndef HW1_UNIONFIND_H
#define HW1_UNIONFIND_H
#include <iostream>
#include "exceptions.h"
#define EMPTY -1
//todo: ranktree should contain levelzero counter

//class Group {
//    RankTree *scoresArray;
//    int scale;
//    bool isEmpty;
//    int size;
//
//
//public:
//    Group(int scale) : scale(scale), isEmpty(true),size(0) {
//        scoresArray = new RankTree [scale];
//        for (int i = 0; i < scale; ++i) {
//            scoresArray[i] = nullptr;
//        }
//    }
//
//    ~Group() {
//        for (int i = 0; i < scale; ++i)
//            delete scoresArray[i];
//        delete[] scoresArray;
//    }
//
//    void mergeGroups(Group &otherGroup){
//        for (int i = 0; i < scale; ++i)
//            scoresArray[i].merge(otherGroup.scoresArray[i]);
//        size+=otherGroup.size;
//         otherGroup.size=1;
//    }
//    int getSize(){
//        return size;
//    }
//};
class Group{
    int size;
    int num;
public:
    Group(int num) : num(num),size(1) {}

    Group(const Group& other ){
        size=other.size;
        num=other.num;
    }

    Group& operator=(const Group& other){
        size=other.size;
        num=other.num;
    }

    virtual ~Group() {

    }

    int getSize(){
        return size;
    }
    void mergeGroups(Group &otherGroup){
        size+=otherGroup.size;
//        otherGroup.size=1;
    }
};
class UnionFind {
    Group** groups;
    int *parent;
    int numOfGroups;


    void updateFather(int i, int father) {
        if (parent[i] == father || parent[i] == EMPTY)
            return;
        parent[i] = father;
        updateFather(parent[i], father);
        return;
    }

public:
    UnionFind(int k, int scale) : numOfGroups(k) {
        groups = new Group*[k];
        parent = new int[k];
        if (!groups || !parent) {
            delete[] groups;
            delete[] parent;
            throw std::bad_alloc();
        }
        for (int i = 0; i < k; ++i) {
            parent[i] = EMPTY;
            groups[i] = new Group(scale);
        }
    }

    ~UnionFind() {
        for (int i = 0; i < numOfGroups; ++i) {
            delete groups[i];
        }
        delete[] groups;
        delete[] parent;
    }

    void Union(int A, int B) {
        int groupA= find(A);
        int groupB= find(B);
        if (groups[groupA]->getSize() < groups[groupB]->getSize()) {
            parent[groupA] = groupB;
            groups[groupB]->mergeGroups(*groups[groupA]);
        }
            else{
            parent[groupB] = groupA;
                groups[groupA]->mergeGroups(*groups[groupB]);
            }

    }


    int find(int i) {
        int father = internal_find(i);
        updateFather(i, father);
        return father;

    }

    int internal_find(int i) {
        if (parent[i] == EMPTY)
            return i;
        find(parent[i]);
    }



    friend std::ostream &operator<<(std::ostream &os, const UnionFind& current) {
        for (int i = 0; i < current.numOfGroups; ++i) {
            os << i << " is son of " << current.parent[i] << " his size is: " << current.groups[i]->getSize() << std::endl;
        }
        os << std::endl;
        return os;
    }

};


#endif //HW1_UNIONFIND_H
