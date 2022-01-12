//
// Created by avi on 06/01/2022.
//

#ifndef HW2_PLAYERBYID_H
#define HW2_PLAYERBYID_H

class PlayerByID{
    int id;
    int groupID;
    int score;
    int level;
public:
    PlayerByID(int id, int groupId, int score, int level) : id(id), groupID(groupId), score(score), level(level) {}

    int getId() const {
        return id;
    }

    void setScore(int scoreToChange) {
        score = scoreToChange;
    }

    void addToLevel(int levelIncrease) {
        level += levelIncrease;
    }

    int getGroupId() const {
        return groupID;
    }

    int getScore() const {
        return score;
    }

    int getLevel() const {
        return level;
    }

    bool operator>(const PlayerByID& other){
        return id > other.id;
    }

    bool operator<(const PlayerByID& other){
        return id < other.id;
    }

    bool operator==(const PlayerByID& other){
        return id == other.id;
    }
};

#endif //HW2_PLAYERBYID_H
