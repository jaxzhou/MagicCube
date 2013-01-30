//
//  PlayerData.h
//  MagCube
//
//  Created by zhou jinxiu on 13-1-24.
//
//

#ifndef __MagCube__PlayerData__
#define __MagCube__PlayerData__

#include <iostream>

class PlayerData{
public:
    static PlayerData *currentUserData();
public:
    PlayerData(void);
    ~PlayerData(void);
    unsigned long userScore;
    void reset();
private:
    
};

#endif /* defined(__MagCube__PlayerData__) */
