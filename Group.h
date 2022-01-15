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
//scores[0] is the groupInfo for all players in the group in all scores.
public:
    Group(int scale) : scale(scale), size(0), scores(new GroupInfoByScore[scale + 1]) {
    }

    ~Group() {
        delete[] scores;
    }

    void insertPlayer(int level, int score) {
        validate();
        if (level == 0) {
            scores[score].increaseLevelZeroCounter();
            scores[0].increaseLevelZeroCounter();
        } else {
            scores[score].addPlayerWithLevel(level);
            scores[0].addPlayerWithLevel(level);
        }
        size++;
        validate();
    }

    void removePlayer(int level, int score) {
        validate();
        scores[score].validateLevelTree();
        scores[0].validateLevelTree();
        if (level == 0) {
            scores[score].decreaseLevelZeroCounter();
            scores[0].decreaseLevelZeroCounter();
        } else {
            scores[score].removePlayerWithLevel(level);
            scores[0].removePlayerWithLevel(level);
        }
        size--;
        validate();
    }

    void mergeGroups(Group *otherGroup) {
        validate();
        for (int i = 0; i <= scale; ++i) {
            scores[i].merge(otherGroup->scores[i]);
            scores[i].addLevelZeroCounter(otherGroup->scores[i].getLevelZeroCounter());
        }
        size += otherGroup->size;
        validate();
    }

    int getSize() const {
        return size;
    }

    int playersCountBetween(int lowLevel, int highLevel) {
        return playersCountBetweenInScore(lowLevel, highLevel, 0);
    }

    int playersCountBetweenInScore(int lowLevel, int highLevel, int score) {
        return scores[score].countScoreBetween(lowLevel, highLevel);
    }


    double getTopMAverage(int m) {
        return scores[0].getTopM(m);
    }

    void validate() {
#ifdef DEBUG
        for (int i = 0; i <= scale; i++) {
            scores[i].validateLevelTree();
        }
        int counter = 0;
        int max = scores[0].getMaxLevel();
        for (int i = 1; i <= scale; ++i) {
            if (scores[i].getMaxLevel() > max)
                assert(0);
        }
        for (int i = 1; i <= scale; ++i) {
            counter += playersCountBetweenInScore(0, max, i);
        }
        assert(counter == playersCountBetween(0, max));
#endif
    }
};


#endif //HW2_GROUP_H
