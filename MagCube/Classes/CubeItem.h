//
//  CubeItem.h
//  MagCube
//
//  Created by zhou jinxiu on 12-10-22.
//
//

#ifndef __MagCube__CubeItem__
#define __MagCube__CubeItem__

#include <iostream>
#include <cocos2d.h>

struct cubeType {
    int color;
    int content;
};

class CubeItem : public cocos2d::CCSprite
{
public:
    static CubeItem *create();
public:
    virtual void draw(void);
    virtual bool init();
    void moveTo(cocos2d::CCPoint to,cocos2d::CCCallFunc *calfuc);
    void destroy();
    cubeType type;
    cocos2d::CCPoint pos;
    bool needMove;
    bool needAdd;
private:
    cocos2d::CCTexture2D *contentTexture;
    cocos2d::CCTexture2D *colorTexture;
};

#endif /* defined(__MagCube__CubeItem__) */
