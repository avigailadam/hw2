//
// Created by avi on 04/01/2022.
//

#ifndef HW2_GAME_H
#define HW2_GAME_H


class Game {
public:
    void mergeGroups(int group1ID, int group2ID);
    void addPlayer(int playerID, int GroupID, int score);
    void removePlayer(int playerID);
    void increasePlayerLevel(int playerID, int levelIncrease);
    void changePlayerScore(int playerID, int newScore);
    double getPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel);
    double averageHighestPlayerLevelByGroup(int groupID, int m);

};


#endif //HW2_GAME_H
