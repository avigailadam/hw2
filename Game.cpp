//
// Created by avi on 04/01/2022.
//

#include "Game.h"

Game::Game(int k, int scale) : k(k), scale(scale), players(new HashTable), groups(new UnionFind(k, scale)) {
    if (scale > 200 || scale <= 0 || k <= 0)
        throw InvalidInput();
}

void Game::addPlayer(int playerID, int groupId, int score) {
    if (groupId <= 0 || groupId > k || playerID <= 0 || score > scale || score <= 0)
        throw InvalidInput();
    if (players->exists(playerID))
        throw AlreadyExist();
    players->insert(playerID, groupId, score);
    Group *group = groups->getGroup(groupId);
    group->insertPlayer(0, score);
    Group *all = groups->getGroup(0);
    all->insertPlayer(0, score);
}

void Game::mergeGroups(int group1ID, int group2ID) {
    if (group1ID <= 0 || group1ID > k || group2ID <= 0 || group2ID > k)
        throw InvalidInput();
    groups->Union(group1ID, group2ID);
}

void Game::removePlayer(int playerID) {
    if (playerID <= 0)
        throw InvalidInput();
    PlayerByID player = players->getPlayer(playerID);
    Group *group = groups->getGroup(player.getGroupId());
    group->removePlayer(player.getLevel(), player.getScore());
    Group *all = groups->getGroup(0);
    all->removePlayer(player.getLevel(), player.getScore());
}

void Game::increasePlayerLevel(int playerID, int levelIncrease) {
    if (playerID <= 0 || levelIncrease <= 0)
        throw InvalidInput();
    PlayerByID &player = players->getPlayer(playerID);
    Group *group = groups->getGroup(player.getGroupId());
    group->removePlayer(player.getLevel(), player.getScore());
    group->insertPlayer(levelIncrease + player.getLevel(), player.getScore());
    Group *all = groups->getGroup(0);
    all->removePlayer(player.getLevel(), player.getScore());
    all->insertPlayer(levelIncrease + player.getLevel(), player.getScore());
    player.addToLevel(levelIncrease);
}

void Game::changePlayerScore(int playerID, int newScore) {
    if (newScore <= 0 || newScore > scale)
        throw InvalidInput();
    PlayerByID &player = players->getPlayer(playerID);
    int oldScore = player.getScore();
    if (oldScore == newScore)
        return;
    player.setScore(newScore);
    Group *group = groups->getGroup(player.getGroupId());
    int level = player.getLevel();
    group->removePlayer(level, oldScore);
    group->insertPlayer(level, newScore);
    Group *all = groups->getGroup(0);
    all->removePlayer(level, oldScore);
    all->insertPlayer(level, newScore);
}

double Game::getPercentOfPlayersWithScoreInBounds(int groupID, int score, int lowerLevel, int higherLevel) {
    if (groupID > k || groupID < 0)
        throw InvalidInput();
    if (score > scale || score <= 0 || lowerLevel < 0 || higherLevel < 0 || lowerLevel > higherLevel)
        return 0;
    Group *group = groups->getGroup(groupID);
    int inScore = group->playersCountBetweenInScore(lowerLevel, higherLevel, score);
    int inGeneral = group->playersCountBetween(lowerLevel, higherLevel);
    return inGeneral == 0? 0:((double(inScore)) / inGeneral) * 100;
}


double Game::averageHighestPlayerLevelByGroup(int groupID, int m) {
    if (groupID > k || groupID < 0 || m <= 0)
        throw InvalidInput();
    Group *group = groupID == 0 ? groups->getGroup(0) : groups->getGroup(groupID);
    if (m >= group->getSize())
        throw OutOfRange();
    return group->getTopMAverage(m);

}