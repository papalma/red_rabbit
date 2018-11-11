//
//  PauseScene.hpp
//  TestGame
//
//  Created by Patricio Palma Contreras on 11/10/18.
//

#ifndef PauseScene_H
#define PauseScene_H

#include "cocos2d.h"

USING_NS_CC;

class PauseScene : public Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void exitPause(Ref* pSender);
    CREATE_FUNC(PauseScene);
private:
    Director* _director;
    Size _visibleSize;
};

#endif
