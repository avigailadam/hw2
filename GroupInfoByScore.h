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

    LevelNode(int level, int numOfPlayers) : level(level), playersCounterInNode(numOfPlayers) {
        assert(level != 0);
    }

    LevelNode& operator=(const LevelNode& other) = default;
    LevelNode(const LevelNode& other) = default;

    int getSelfSum() const {
        return playersCounterInNode;
    }

    LevelNode operator+(const LevelNode &other) const {
        assert(this->level == other.level);
        LevelNode result = *this;
        result.playersCounterInNode += other.playersCounterInNode;
        return result;
    }

    int getLevel() const {
        return level;
    }

    friend std::ostream &operator<<(std::ostream &os, const LevelNode &node) {
        os << "level: " << node.level << " playersCounterInNode: " << node.playersCounterInNode;
        return os;
    }

    void decreaseCounter() {
        playersCounterInNode--;
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

    virtual ~GroupInfoByScore() {
        delete levelTree;
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
        if (!levelTree->contains(tmpNode)) {
            levelTree->insert(tmpNode);
            return;
        }
        LevelNode &node = levelTree->find(tmpNode);
        LevelNode newNode = LevelNode(level, node.getSelfSum() + 1);
        levelTree->remove(node);
        levelTree->insert(newNode);
    }

    void removePlayerWithLevel(int level) {
        levelTree->validate();
        LevelNode tmpNode(level);
        LevelNode &node = levelTree->find(tmpNode);
        levelTree->validate();
        node.decreaseCounter();
        if (node.getSelfSum() == 0) {
            levelTree->remove(node);
            return;
        }
        LevelNode newNode = LevelNode(level, node.getSelfSum());
        levelTree->remove(node);
        levelTree->insert(newNode);
    }

    double getTotSum(int m) {
        return levelTree->findTopMMult(m);
    }

    void validateLevelTree() {
        levelTree->validate();
    }

    void merge(const GroupInfoByScore &other) {
        levelTree->merge(other.levelTree);
    }

    int countScoreBetween(int lowLevel, int highLevel) {
        if (levelTree->isEmpty() || highLevel == 0)
            return lowLevel <= 0 ? levelZeroCounter : 0;
        LevelNode high(highLevel + 1);
        int sumOverHigh = levelTree->totalSumOver(high);
        int tmpLow = lowLevel == 0 ? 1 : lowLevel;
        int sumOverLow = levelTree->totalSumOver(tmpLow);
        if (lowLevel == 0)
            sumOverLow += levelZeroCounter;
        return sumOverLow - sumOverHigh;
    }

    double getTopM(int m) {
        if (levelTree->getSum() >= m)
            return getTotSum(m) / m;
        return levelTree->isEmpty()
               ? 0.0
               : double(levelTree->getPeopleMultipliedByLevel()) / m;
    }

    int getMaxLevel() const{
        return levelTree->isEmpty() ? 0 : levelTree->getMax().getLevel();
    }

    void printTree(){
        levelTree->debugTree();
    }
};

#endif //HW2_GROUPINFOBYSCORE_H
