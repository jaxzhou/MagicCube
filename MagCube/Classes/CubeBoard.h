//
//  CubeBoard.h
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#ifndef __MagCube__CubeBoard__
#define __MagCube__CubeBoard__

#include <iostream>
#include <cocos2d.h>
#include "CubeItem.h"

class CubeBoard 
{
public:
    CubeBoard(void);
    ~CubeBoard(void);
    void generator();
    void coutClear();
    void clearAndMove();
    CubeItem *itemAtPos(int x,int y);
    cocos2d::CCPoint posOfItem(CubeItem *item);
    cocos2d::CCPointArray *matchedArray;
    std::vector<CubeItem*> clearedItems;
    void selectPositon(cocos2d::CCPoint pos);
    bool ShouldCleaer();
private:
    cocos2d::CCPointArray *selectedArray;
    cocos2d::CCPointArray *clearArray;
    std::vector<std::vector<CubeItem*> > itemMatrix;
    bool _shouldClear;
};

class BoardLayout
{
public:
    static BoardLayout *defaultLayout();
public:
    BoardLayout(void);
    ~BoardLayout(void);
    cocos2d::CCRect boardRect;
    cocos2d::CCRect getRectOfPostion(int i,int j);
    cocos2d::CCPoint dimOfPostion(cocos2d::CCPoint pos);
protected:
    float _width;
    float _height;
};


#endif /* defined(__MagCube__CubeBoard__) */
