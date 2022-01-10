//
// Created by avi on 06/01/2022.
//

#ifndef HW2_GROUPINFOBYSCORE_H
#define HW2_GROUPINFOBYSCORE_H

#include <ostream>
#include "RankTree.h"

class LevelNode {
    int level;
    int playersCounterInNode;
public:
    LevelNode() : level(-1), playersCounterInNode(-1) {}

    LevelNode(int level) : level(level), playersCounterInNode(1) {
        assert(level != 0);
    }

    int getSum() const {
        return playersCounterInNode;
    }

    int getLevel() const {
        return level;
    }

    friend std::ostream &operator<<(std::ostream &os, const LevelNode &node) {
        os << "level: " << node.level << " playersCounterInNode: " << node.playersCounterInNode;
        return os;
    }

    LevelNode(const LevelNode &other) = default;

    void increaseCounter() {
        playersCounterInNode++;
    }

    void decreaseCounter() {
        playersCounterInNode--;
    }

    int getCounter() const {
        return playersCounterInNode;
    }

    bool operator>(const LevelNode &other) const {
        return level > other.level;
    }

    bool operator<(const LevelNode &other) const {
        return level < other.level;
    }

    bool operator==(const LevelNode &other) const {
        return level == other.level;
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
        LevelNode &node = tmpNode;
        if (!levelTree->contains(tmpNode)) {
            levelTree->insert(tmpNode);
            return;
        }
        levelTree->find(tmpNode).increaseCounter();
    }

    void removePlayerWithLevel(int level) {
        LevelNode tmpNode(level);
        LevelNode &node = levelTree->find(tmpNode);
        node.decreaseCounter();
        if(node.getSum() == 0)
            levelTree->remove(node);
    }

    int getNumOfPlayersAtZero() {
        return levelZeroCounter;
    }

    int getTreeSize() {
        return levelTree->getSum();
    }

    double getTotSum(int m) {
        return levelTree->findTopMMult(m);
    }

};

#endif //HW2_GROUPINFOBYSCORE_H
