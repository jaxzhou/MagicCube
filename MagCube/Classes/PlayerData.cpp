//
//  PlayerData.cpp
//  MagCube
//
//  Created by zhou jinxiu on 13-1-24.
//
//

#include "PlayerData.h"

static PlayerData* _instance = NULL;
PlayerData* PlayerData::currentUserData(){
    if (_instance==NULL) {
        _instance = new PlayerData();
    }
    return _instance;
};


PlayerData::PlayerData(void):userScore(0){
    
}

PlayerData::~PlayerData(void){
    
}

void PlayerData::reset(){
    userScore = 0;
}