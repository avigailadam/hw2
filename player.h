//
// Created by hagai on 1/4/2022.
//

#ifndef HW2_PLAYER_H
#define HW2_PLAYER_H
class playerByLevel{
    int groupID;
    int level;
    int playerID;
    int score;

public:
    playerByLevel(int groupID,int playerID,int score): groupID(groupID), playerID(playerID),score(score),level(0){}

    ~playerByLevel()=defau0l0t;

    bool operator<(const playerByLevel& other){
        if(level==other.level)
            return playerID<other.playerID;
        return level<other.level;
    }
    bool operator==(const playerByLevel& other){
        return playerID==other.playerID;
    }
    void setLevel(int lev) {
        level = lev;
    }
    int getLevel() const {
        return level;
    }
    int getScore(){
        return score;
    }
};
#endif //HW2_PLAYER_H
