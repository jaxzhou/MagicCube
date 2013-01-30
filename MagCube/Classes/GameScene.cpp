//
//  GameScene.cpp
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CubeItem.h"
#include "PlayerData.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

CCScene * CubeGame::scene()
{
    CCScene *scene = CCScene::create();
    
    CubeGame *layer = CubeGame::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool CubeGame::init()
{
    //super init
    if ( !CCLayer::init() )
    {
        return false;
    }
    //init
    board = new CubeBoard();
    BoardLayout::defaultLayout()->boardRect = CCRect(10, 0, 320, 320);
    CubeItem *item = CubeItem::create();
    item->setPosition(CCPoint(20, 20));
    this->addChild(item);
    item->moveTo(CCPoint(100, 10), CCCallFunc::create(this, callfunc_selector(CubeGame::actionFinished)));
    //
    PlayerData::currentUserData()->reset();
    selectedPos.x = -1;
    selectedPos.y = -1;
    scheduleUpdate();
    setTouchEnabled(true);
    runingActions = 0;
    activeArray = CCPointArray::create(8*8);
    activeArray->retain();
    return true;
}

void CubeGame::actionFinished()
{
    runingActions-=1;
    if (runingActions==0 ) {
        if (state==gameInitial) {
            state = gameCounting;
        }else if(state == gameClearing){
            state = gameFilling;
        }
    }else if(runingActions<0){
        runingActions = 0;
    }
}

void CubeGame::update(float dt)
{
    if (state == gameLoading) {
        board->generator();
        state = gameInitial;
    }
    else if (state == gameCounting) {
        if (board->ShouldCleaer()) {
            board->clearAndMove();
            state = gameClearing;
        }
    }
    else if(state == gameFilling){
        selectedPos.x = -1;
        selectedPos.y = -1;
        state = gameCounting;
    }
}

void CubeGame::draw()
{
    if (state == gameInitial) {
        if (runingActions == 0) {
            for (int i = 0; i < 8; i ++) {
                for (int j = 0; j < 8; j ++) {
                    CubeItem *item = board->itemAtPos(i, j);
                    if (item!=NULL && item->needMove) {
                        runingActions+=1;
                        if (item->needAdd) {
                            this->addChild(item);
                            item->needAdd=false;
                        }
                        item->moveTo(item->pos, CCCallFunc::create(this, callfunc_selector(CubeGame::actionFinished)));
                    }
                }
            }
        }
    }
    else if (state==gameCounting) {
        if (selectedPos.x > 0 && selectedPos.y > 0) {
            CubeItem *selectedItem = board->itemAtPos(selectedPos.x, selectedPos.y);
            selectedArray(selectedItem->type.content, selectedPos);
            for (int i = 0; i < activeArray->count(); i ++) {
                CCPoint pos = activeArray->getControlPointAtIndex(i);
                CCRect bacRect = BoardLayout::defaultLayout()->getRectOfPostion(pos.x, pos.y);
                ccDrawSolidRect(bacRect.origin, CCPoint(bacRect.origin.x+bacRect.size.width, bacRect.origin.y+bacRect.size.height), ccc4f(1,1,1,1));
            }
        }
    }else if(state == gameClearing){
        if (runingActions == 0) {
            PlayerData::currentUserData()->userScore += board->clearedItems.size()*5;
            for(int i = 0;i < board->clearedItems.size(); i ++)
            {
                CubeItem *item = board->clearedItems[i];
                this->removeChild(item, true);
            }
            for (int i = 0; i < 8; i ++) {
                for (int j = 0; j < 8; j ++) {
                    CubeItem *item = board->itemAtPos(i, j);
                    if (item!=NULL && item->needMove) {
                        runingActions++;
                        if (item->needAdd) {
                            this->addChild(item);
                            item->needAdd=false;
                        }
                        item->moveTo(item->pos, CCCallFunc::create(this, callfunc_selector(CubeGame::actionFinished)));
                    }
                }
            }
        }
    }
    this->drawScoreBoard();
}

void CubeGame::drawScoreBoard(){
    CCTexture2D *scoreTexture = new CCTexture2D();
    stringstream ss;
    ss<<PlayerData::currentUserData()->userScore;
    scoreTexture->initWithString(ss.str().c_str(),"",15 );
    scoreTexture->drawAtPoint(ccp(50, 360));
}


void CubeGame::ccTouchesBegan(cocos2d::CCSet *touches,cocos2d::CCEvent *event){
    cocos2d::CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint touchPos = touch->getLocation();
    CCPoint dim = BoardLayout::defaultLayout()->dimOfPostion(touchPos);
    if (dim.x> -1 && dim.y > -1) {
        if (activeArray->count()>0 && selectedPos.x > -1 && selectedPos.y > -1) {
            for (int i = 0; i < activeArray->count(); i ++) {
                if ((activeArray->getControlPointAtIndex(i)).equals(dim)) {
                    CubeItem *selectedItem = board->itemAtPos(selectedPos.x, selectedPos.y);
                    CubeItem *currentITem = board->itemAtPos(dim.x, dim.y);
                    if(selectedItem && selectedItem->type.color == currentITem->type.color){
                        board->selectPositon(dim);
                        selectedPos = dim;
                        break;
                    }
                }
            }
        }
        else
        {
            board->selectPositon(dim);
            selectedPos = dim;
        }
    }
}

void CubeGame::selectedArray(int type, CCPoint pos)
{
    activeArray->release();
    activeArray = NULL;
    activeArray = CCPointArray::create(8*8);
    switch (type) {
        case 0:
        {
            for (int i = 0; i < 8; i ++) {
                CCPoint ap = CCPoint(pos.x, i);
                CCPoint ap2 = CCPoint(i,pos.y);
                if (!ap.equals(pos)) {
                    activeArray->addControlPoint(ap);
                }
                if (!ap2.equals(pos)) {
                    activeArray->addControlPoint(ap2);
                }
            }
        }
            break;
        case 1:
        {
            for (int i = 0; i < 9; i ++) {
                CCPoint ap = CCPoint(pos.x-1+i%3,pos.y-1+i/3);
                if (!ap.equals(pos)) {
                    activeArray->addControlPoint(ap);
                }
            }
        }
            break;
        case 2:
        {
            CCRect boardRect = CCRect(0, 0, 7, 7);
            for (int i = 1; i < 8; i ++) {
                CCPoint a1 = CCPoint(pos.x+i,pos.y+i);
                CCPoint a2 = CCPoint(pos.x-i,pos.y+i);
                CCPoint a3 = CCPoint(pos.x+i,pos.y-i);
                CCPoint a4 = CCPoint(pos.x-i,pos.y-i);
                if (boardRect.containsPoint(a1)) {
                    activeArray->addControlPoint(a1);
                }
                if (boardRect.containsPoint(a2)) {
                    activeArray->addControlPoint(a2);
                }
                if (boardRect.containsPoint(a3)) {
                    activeArray->addControlPoint(a3);
                }
                if (boardRect.containsPoint(a4)) {
                    activeArray->addControlPoint(a4);
                }
            }
        }
            break;
        case 3:
        {
            CCRect boardRect = CCRect(0, 0, 7, 7);
            for (int i = 0; i < 8; i ++) {
                if (i > 0) {
                    CCPoint a1 = CCPoint(pos.x+i,pos.y+i);
                    CCPoint a2 = CCPoint(pos.x-i,pos.y+i);
                    CCPoint a3 = CCPoint(pos.x+i,pos.y-i);
                    CCPoint a4 = CCPoint(pos.x-i,pos.y-i);
                    if (boardRect.containsPoint(a1)) {
                        activeArray->addControlPoint(a1);
                    }
                    if (boardRect.containsPoint(a2)) {
                        activeArray->addControlPoint(a2);
                    }
                    if (boardRect.containsPoint(a3)) {
                        activeArray->addControlPoint(a3);
                    }
                    if (boardRect.containsPoint(a4)) {
                        activeArray->addControlPoint(a4);
                    }
                }
                
                CCPoint a5 = CCPoint(i,pos.y);
                CCPoint a6 = CCPoint(pos.x,i);
                if (!a5.equals(pos)) {
                    activeArray->addControlPoint(a5);
                }
                if (!a6.equals(pos)) {
                    activeArray->addControlPoint(a6);
                }
            }
        }
            break;
        case 4:
        {
            CCRect boardRect = CCRect(0, 0, 7, 7);
            for (int i = 1; i < 8-pos.y; i ++) {
                CCPoint a1 = CCPoint(pos.x,pos.y+i);
                CCPoint a2 = CCPoint(pos.x+i,pos.y+i);
                CCPoint a3 = CCPoint(pos.x-i,pos.y+i);
                if (boardRect.containsPoint(a1)) {
                    activeArray->addControlPoint(a1);
                }
                if (boardRect.containsPoint(a2)) {
                    activeArray->addControlPoint(a2);
                }
                if (boardRect.containsPoint(a3)) {
                    activeArray->addControlPoint(a3);
                }
            }
        }
            break;
        case 5:
        {
            CCRect boardRect = CCRect(0, 0, 7, 7);
            for (int i = 1; i <= pos.y; i ++) {
                CCPoint a1 = CCPoint(pos.x,pos.y-i);
                CCPoint a2 = CCPoint(pos.x+i,pos.y-i);
                CCPoint a3 = CCPoint(pos.x-i,pos.y-i);
                if (boardRect.containsPoint(a1)) {
                    activeArray->addControlPoint(a1);
                }
                if (boardRect.containsPoint(a2)) {
                    activeArray->addControlPoint(a2);
                }
                if (boardRect.containsPoint(a3)) {
                    activeArray->addControlPoint(a3);
                }
            }
        }
            break;
        default:
            break;
    }
    activeArray->retain();
}


