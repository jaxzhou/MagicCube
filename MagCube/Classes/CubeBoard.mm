//
//  CubeBoard.cpp
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#include "CubeBoard.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;


CubeBoard::CubeBoard(void)
{
    selectedArray = CCPointArray::create(8*8);
    _shouldClear = false;
}

CubeBoard::~CubeBoard(void)
{
    itemMatrix.clear();
}

void CubeBoard::generator()
{
    for (int i = 0; i < 8; i ++) {
        vector<CubeItem*> itemList;
        itemMatrix.push_back(itemList);
        for (int j = 0; j < 8; j ++) {
            CubeItem *item = CubeItem::create();
            item->pos.x = i;
            item->pos.y = j;
            item->needMove = true;
            item->needAdd = true;
            item->setPosition(BoardLayout::defaultLayout()->getRectOfPostion(i, j + 8).origin);
            itemMatrix[i].push_back(item);
        }
    }
    selectedArray = CCPointArray::create(8*8);
    selectedArray->retain();
}

void CubeBoard::coutClear()
{
    clearArray = CCPointArray::create(64);
    CGMutablePathRef path = CGPathCreateMutable();
    for (int i = 0; i < selectedArray->count(); i ++) {
        CCPoint pos = selectedArray->getControlPointAtIndex(i);
        if (i==0) {
            CGPathMoveToPoint(path, NULL, pos.x, pos.y);
        }
        CGPathAddLineToPoint(path, NULL, pos.x, pos.y);
    }
    
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++){
            CCPoint pos = CCPoint(float(i),float(j));
            if (CGPathContainsPoint(path, NULL, CGPointMake(pos.x,pos.y), true)) {
                clearArray->addControlPoint(pos);
            }
        }
    }
}

void CubeBoard::clearAndMove()
{
    this->coutClear();
    for (int i = 0; i < clearArray->count(); i ++) {
        CCPoint pos = clearArray->getControlPointAtIndex(i);
        CubeItem* t = itemMatrix[pos.x][pos.y];
        //t->destroy();
        
        clearedItems.push_back(t);
        itemMatrix[pos.x][pos.y] = NULL;
    }
    for (int i = 0; i < 8; i ++) {
        int move = 0;
        for (int j = 0; j < 8; j ++) {
            if (itemMatrix[i][j] == NULL) {
                move ++;
            }else if(move > 0){
                CubeItem* t = itemMatrix[i][j];
                itemMatrix[i][j-move] = t;
                t->pos.x = i;
                t->pos.y = j-move;
                t->needMove = true;
                itemMatrix[i][j] = NULL;
            }
        }
        for (int j = 0; j < 8; j ++) {
            if (itemMatrix[i][j] == NULL) {
                CubeItem *item = CubeItem::create();
                item->pos.x = i;
                item->pos.y = j;
                item->needMove = true;
                item->needAdd = true;
                item->setPosition(BoardLayout::defaultLayout()->getRectOfPostion(i, j + move).origin);
                itemMatrix[i][j] = item;
            }
        }
    }
    selectedArray->release();
    selectedArray = CCPointArray::create(8*8);
    selectedArray->retain();
    _shouldClear = false;
}

bool CubeBoard::ShouldCleaer()
{
    return _shouldClear;
}

void CubeBoard::selectPositon(cocos2d::CCPoint pos)
{
    for (int i = 0; i < selectedArray->count(); i ++) {
        if (selectedArray->getControlPointAtIndex(i).equals(pos)) {
            _shouldClear = true;
        }
    }
    selectedArray->addControlPoint(pos);
}

cocos2d::CCPoint CubeBoard::posOfItem(CubeItem *item){
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++) {
            if (itemMatrix[i][j]==item) {
                return CCPoint(float(i),float(j));
            }
        }
    }
    return CCPoint(-1, -1);
}

CubeItem * CubeBoard::itemAtPos(int x,int y){
    if (x < itemMatrix.size() && y < itemMatrix[x].size()) {
        return itemMatrix[x][y];
    }
    return NULL;
}

static BoardLayout *layout = NULL;
BoardLayout *BoardLayout::defaultLayout()
{
    if (layout == NULL) {
        layout = new BoardLayout();
    }
    return layout;
}

BoardLayout::BoardLayout(void):_width(8),_height(8)
{
    
}

BoardLayout::~BoardLayout(void)
{
    
}

cocos2d::CCRect BoardLayout::getRectOfPostion(int i,int j)
{
    float w = boardRect.size.width/_width;
    float h = boardRect.size.height/_height;
    return CCRect(w*i, h*j, w, h);
}

CCPoint BoardLayout::dimOfPostion(CCPoint pos)
{
    float w = boardRect.size.width/_width;
    float h = boardRect.size.height/_height;
    int x = pos.x/w;
    int y = pos.y/h;
    if (x<0 || x > _width) {
        x = -1;
    }
    if (y < 0 || y > _height) {
        y = -1;
    }
    return CCPoint(x,y);
}


