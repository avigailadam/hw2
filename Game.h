//
// Created by avi on 04/01/2022.
//

#ifndef HW2_GAME_H
#define HW2_GAME_H
#include "HashTable.h"
#include "UnionFind.h"


class Game {
    int k;
    int scale;
    HashTable* players;
    UnionFind* groups;
public:
    Game(int k, int scale);

    void mergeGroups(int group1ID, int group2ID);
    void addPlayer(int playerID, int groupId, int score);
    void removePlayer(int playerID);
    void increasePlayerLevel(int playerID, int levelIncrease);
    void changePlayerScore(int playerID, int newScore);
    double getPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel);
    double averageHighestPlayerLevelByGroup(int groupID, int m);

};


#endif //HW2_GAME_H
