//
//  GameOverScene.h
//  TestGame
//
//  Created by Patricio Palma Contreras on 11/10/18.
//

#ifndef GameOverScene_H
#define GameOverScene_H

#include "cocos2d.h"

USING_NS_CC;

class GameOverScene : public Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void exitPause(Ref* pSender);
    CREATE_FUNC(GameOverScene);
private:
    Director* _director;
    Size _visibleSize;
};

#endif
