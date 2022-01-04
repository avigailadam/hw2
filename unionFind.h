//
// Created by hagai on 1/3/2022.
//

#ifndef HW1_UNIONFIND_H
#define HW1_UNIONFIND_H
#define EMPTY -1
//todo: ranktree should contain levelzero counter

class Group {
    RankTree *scoresArray;
    int scale;
    bool isEmpty;
    int size;


public:
    Group(int scale) : scale(scale), isEmpty(true),size(0) {
        scoresArray = new RankTree [scale];
        for (int i = 0; i < scale; ++i) {
            scoresArray[i] = nullptr;
        }
    }

    ~Group() {
        for (int i = 0; i < scale; ++i)
            delete scoresArray[i];
        delete[] scoresArray;
    }

    void mergeGroups(Group &otherGroup){
        for (int i = 0; i < scale; ++i)
            scoresArray[i].merge(otherGroup.scoresArray[i]);
        size+=otherGroup.size;
    }
    int getSize(){
        return size;
    }
};

class UnionFind {
    Group *groups;
    int *parents;
    int numOfGroups;
public:
    UnionFind(int k, int scale) : numOfGroups(k) {
        if (k <= 0 || scale > 200 || scale <= 0)
            return nullptr;
        groups = new Group[k];
        parents = new int[k];
        if (!groups || !parents) {
            delete[] groups;
            delete[] parents;
            throw bad_alloc();
        }
        for (int i = 0; i < k; ++i) {
            parents[i] = EMPTY;
            groups[i] = new Group(scale);
        }
    }

    ~UnionFind() {
        for (int i = 0; i < numOfGroups; ++i) {
            delete groups[i];
        }
        delete[] groups;
        delete[] parents;
    }

    void Union(int group_A, int group_B) {
        if (groups[group_A].getSize() < groups[group_B].getSize()) {
            parents[group_A] = group_B;
            //todo: merge trees insert A to B
            groups[group_B].mergeGroups(groups(group_A));
            else{
                parents[group_B] = group_A;
                //todo: merge trees insert B to A
                groups[group_A].mergeGroups(groups(group_B));
            }
        }

    }

    int find(int i) {
        int father = internal_find(i);
        updateFather(i, father);
        return father;

    }

    int internal_find(int i) {
        if (parents[i] == EMPTY)
            return i;
        find(parents[i]);
    }

    void updateFather(int i, int father) {
        if (parents[i] == father || parents[i] == EMPTY)
            return;
        parents[i] = father;
        updateFather(parents[i], father);
        return;
    }

};


#endif //HW1_UNIONFIND_H
