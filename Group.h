//
// Created by avi on 06/01/2022.
//

#ifndef HW2_GROUP_H
#define HW2_GROUP_H

#include "GroupInfoByScore.h"

class Group {
    int scale;
    int size;
    GroupInfoByScore *scores;
//scores[0] is the group for all players in the group,not devided by score.
public:
    Group(int scale) : scale(scale), size(0), scores(new GroupInfoByScore[scale + 1]) {
    }

    ~Group() {
        delete[] scores;
    }

    void insertPlayer(int level, int score) {
        if (level == 0) {
            scores[score].increaseLevelZeroCounter();
            scores[0].increaseLevelZeroCounter();
        } else {
            scores[score].addPlayerWithLevel(level);
            scores[0].addPlayerWithLevel(level);
        }
        size++;

    }

    void removePlayer(int level, int score) {
        if (level == 0) {
            scores[score].decreaseLevelZeroCounter();
            scores[0].decreaseLevelZeroCounter();
        } else {
            scores[score].removePlayerWithLevel(level);
            scores[0].removePlayerWithLevel(level);
        }
        size--;
    }

    void mergeGroups(Group *otherGroup) {
        for (int i = 0; i <= scale; ++i) {
            (scores[i].getLevelTree())->merge((otherGroup->scores[i]).getLevelTree());
            scores[i].addLevelZeroCounter(otherGroup->scores[i].getLevelZeroCounter());
        }
        size += otherGroup->size;
    }

    int getSize() const {
        return size;
    }

    void increaseLevelZeroCounter() {
        scores->increaseLevelZeroCounter();
    }

    void decreaseLevelZeroCounter() {
        scores->decreaseLevelZeroCounter();
    }

    int playersCountBetween(int lowLevel, int highLevel) {
        int counter = 0;
        for (int i = 1; i <= scale; ++i) {
            counter += playersCountBetweenInScore(lowLevel, highLevel, i);
        }
        return counter;
    }

    int playersCountBetweenInScore(int lowLevel, int highLevel, int score) {
        assert(score != 0);
        GroupInfoByScore info = scores[score];
        LevelNode high(highLevel);
        LevelNode low(lowLevel);
        RankTree<LevelNode>* tree = info.getLevelTree();
        if(tree->isEmpty())
            return lowLevel <= 0 ? info.getLevelZeroCounter() : 0;
        int sumOverHigh = tree->totalSumOver(high);
        int sumOverLow = tree->totalSumOver(low);
        return sumOverLow - sumOverHigh;
    }


    double getTopMAverage(int m){
        if(scores->getTreeSize()>=m)
            return (scores[0].getTotSum(m)/m);
        else{
            return (scores[0].getTotSum(scores->getTreeSize())/m);
        }
 }
};


#endif //HW2_GROUP_H
