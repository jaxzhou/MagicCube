//
//  GameScene.h
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#ifndef __MagCube__GameScene__
#define __MagCube__GameScene__

#include <iostream>
#include <cocos2d.h>
#include "CubeBoard.h"

class CubeGame : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    //virtual void update(float dt);
    CREATE_FUNC(CubeGame);
private:
    //void draw();
    enum GameState{
        gameLoading,
        gameInitial,
        gameCounting,
        gameClearing,
        gameFilling,
        gameOver
    };
    GameState state;
    
    cocos2d::CCPointArray *activeArray;
    CubeBoard *board;
    cocos2d::CCPoint selectedPos;
    void actionFinished();
    int runingActions;
    void update(float dt);
    void draw();
    void drawScoreBoard();
    void ccTouchesBegan(cocos2d::CCSet *touches,cocos2d::CCEvent *event);
    void selectedArray(int type,cocos2d::CCPoint pos);
    int test;
};

#endif /* defined(__MagCube__GameScene__) */
