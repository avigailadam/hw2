//
// Created by hagai on 1/10/2022.

#include "library2.h"
#include "Game.h"


void *Init(int k, int scale) {
    Game *DS;
    try {
        DS = new DS(k, scale);
    } catch (std::bad_alloc &e) {
        return nullptr;
    }
    return (void *) DS;
}


StatusType MergeGroups(void *DS, int GroupID1, int GroupID2) {
    if (DS == nullptr || GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > k || GroupID2 > k)
        return INVALID_INPUT;
    try {
        ((Game *) DS)->mergeGroups(GroupID1, GroupID2);
    } catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    return SUCCESS
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score) {
    if (DS == nullptr) return INVALID_INPUT;
    try { ((Game *) DS)->AddPlayer(PlayerID, GroupID, Level); }
    catch (InvalidInput &res) { return INVALID_INPUT }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (AlreadyExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if (DS == nullptr) return INVALID_INPUT;
    try { ((Game *) DS)->removePlayer(PlayerID); }
    catch (InvalidInput &res) { return INVALID_INPUT; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease) {
    if (DS == nullptr) return INVALID_INPUT;
    try { ((Game *) DS)->increasePlayerLevel(PlayerID, LevelIncrease); }
    catch (InvalidInput &res) { return INVALID_INPUT; }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore) {
    if (DS == nullptr) return INVALID_INPUT;
    try { ((Game *) DS)->changePlayerScore(PlayerID, NewScore); }
    catch (InvalidInput &res) { return INVALID_INPUT; }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double *players) {
    if (DS == nullptr) return INVALID_INPUT;
    try {
        *players = ((Game *) DS)->getPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel,
                                                                       players);
    }
    catch (InvalidInput &res) { return INVALID_INPUT; }
    catch (std::bad_alloc &e) { return ALLOCATION_ERROR; }
    catch (NotExist &res) { return FAILURE; }
    return SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double *level) {
    if (DS == nullptr) return INVALID_INPUT;
    try {
        *level = ((Game *) DS)->averageHighestPlayerLevelByGroup(GroupID, m);
    }
    catch (InvalidInput &res) { return INVALID_INPUT; }
    catch (OutOfRange &res) { return FAILURE; }
    return SUCCESS;
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int *LowerBoundPlayers, int *HigherBoundPlayers);

void Quit(void **DS){
    delete (Game *) *DS;
    *DS = nullptr;
}
