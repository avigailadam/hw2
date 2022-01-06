//
// Created by avi on 06/01/2022.
//

#ifndef HW2_GROUPINFOBYSCORE_H
#define HW2_GROUPINFOBYSCORE_H

#include "RankTree.h"

class LevelNode {
    int level;
    int playersCounterInNode;
public:
    LevelNode(int level) : level(level), playersCounterInNode(1) {
        assert(level != 0);
    }

    int getSum() {
        return playersCounterInNode;
    }

    LevelNode(const LevelNode &other) = default;

    void increaseCounter() {
        playersCounterInNode++;
    }

    void decreaseCounter() {
        playersCounterInNode--;
    }

    int getCounter() {
        return playersCounterInNode;
    }

    int getLevel() {
        return level;
    }

    bool operator>(const LevelNode &other) {
        return level > other.level;
    }

    bool operator<(const LevelNode &other) {
        return level < other.level;
    }

    bool operator==(const LevelNode &other) {
        level == other.level;
    }
};


class GroupInfoByScore {
    RankTree<LevelNode> *levelTree;
    int levelZeroCounter;
public:
    GroupInfoByScore() : levelTree(new RankTree<LevelNode>), levelZeroCounter(0) {}

    RankTree<LevelNode> *getLevelTree() const {
        return levelTree;
    }

    int getLevelZeroCounter() const {
        return levelZeroCounter;
    }

    void addLevelZeroCounter(int num) {
        levelZeroCounter += num;
    }

    void increaseLevelZeroCounter() {
        levelZeroCounter++;
    }

    void decreaseLevelZeroCounter() {
        levelZeroCounter--;
    }

    void addPlayerWithLevel(int level) {
        LevelNode tmpNode(level);
        LevelNode &node = levelTree->find(tmpNode);
        node.increaseCounter();
    }

    void removePlayerWithLevel(int level) {
        LevelNode tmpNode(level);
        LevelNode &node = levelTree->find(tmpNode);
        node.decreaseCounter();
    }
};

#endif //HW2_GROUPINFOBYSCORE_H
