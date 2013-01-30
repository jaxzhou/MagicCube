//
//  CubeItem.cpp
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#include "CubeItem.h"
#include "SimpleAudioEngine.h"
#include "CubeBoard.h"
#include "PlayerData.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

CubeItem* CubeItem::create()
{
    CubeItem *item = new CubeItem;
    item->init();
    return item;
}

bool CubeItem::init()
{
    int colorNum = 3;
    int contentNum = 3;
    
    if (PlayerData::currentUserData()->userScore < 1500) {
        colorNum = 3;
    }else if(PlayerData::currentUserData()->userScore< 3000){
        colorNum = 4;
    }else if(PlayerData::currentUserData()->userScore < 5000){
        colorNum = 5;
    }else if(PlayerData::currentUserData()->userScore < 8000){
        colorNum = 6;
    }else if(PlayerData::currentUserData()->userScore < 12000){
        colorNum = 7;
    }
    
    if (PlayerData::currentUserData()->userScore< 2000) {
        contentNum = 3;
    }else if(PlayerData::currentUserData()->userScore< 5000){
        contentNum = 4;
    }else if(PlayerData::currentUserData()->userScore < 9000){
        contentNum = 5;
    }else if(PlayerData::currentUserData()->userScore < 14000){
        contentNum = 6;
    }
    
    type.color = rand()%colorNum;
    type.content = rand()%contentNum;
    contentTexture = new CCTexture2D();
    switch (type.content) {
        case 0:
        {
            contentTexture->initWithString("+", "Arail", 18);
        }
            break;
        case 1:
        {
            contentTexture->initWithString("o", "Arail", 18);
        }
            break;
        case 2:
        {
            contentTexture->initWithString("X", "Arail", 18);
        }
            break;
        case 3:
        {
            contentTexture->initWithString("*", "Arail", 18);
        }
            break;
        case 4:
        {
            contentTexture->initWithString("V", "Arail", 18);
        }
            break;
        case 5:
        {
            contentTexture->initWithString("n", "Arail", 18);
        }
            break;
            
        default:
            break;
    }
    return true;
}

void CubeItem::moveTo(cocos2d::CCPoint to,cocos2d::CCCallFunc *calfuc)
{
    CCRect moveToRect = BoardLayout::defaultLayout()->getRectOfPostion(to.x, to.y);
    float dur = (this->getPosition().y - moveToRect.origin.y)/320.0;
    dur = dur > 0 ?dur :-dur;
    CCFiniteTimeAction *actionSequence = CCSequence::create(CCMoveTo::create(dur, moveToRect.origin),calfuc,NULL);
    CCDirector::sharedDirector()->getActionManager()->addAction(actionSequence,this, false);
    needMove = false;
}

void CubeItem::draw()
{
    ccColor4F color;
    switch (type.color) {
        case 0:
        {
            color = ccc4f(1.0, 102.0/255, 102.0/255, 1);
        }
            break;
        case 1:
        {
            color = ccc4f(102.0/255, 204.0/255, 1.0, 1);
        }
            break;
        case 2:
        {
            color = ccc4f(1.0, 102.0/255, 1.0, 1);
        }
            break;
        case 3:
        {
            color = ccc4f(1.0, 1.0, 102.0/255, 1);
        }
            break;
        case 4:
        {
            color = ccc4f(1.0, 204.0/255, 102.0/255, 1);
        }
            break;
        case 5:
        {
            color = ccc4f(204.0/255, 102.0/255, 0.0, 1);
        }
            break;
        case 6:
        {
            color = ccc4f(204.0/255, 1.0, 102.0/255, 1);
        }
            break;
            
        default:
            break;
    }
    ccDrawSolidRect(CCPoint(1,1), CCPoint(38,38), color);
    contentTexture->drawAtPoint(CCPoint(5, 5));
}

void CubeItem::destroy(){
    this->removeAllChildrenWithCleanup(true);
}

