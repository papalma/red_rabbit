//
//  PauseScene.cpp
//  TestGame
//
//  Created by Patricio Palma Contreras on 11/10/18.
//

#include "PauseScene.h"

USING_NS_CC;

Scene* PauseScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PauseScene::create();
    scene->addChild(layer);
    return scene;
}

bool PauseScene::init()
{
    if( !Layer::init() ) {
        return false;
    }
    _director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    auto pauseItem = MenuItemImage::create("gtop.jpg",
            "gback.jpg", CC_CALLBACK_1(PauseScene::exitPause, this));

    pauseItem->setPosition(
            Vec2(origin.x + _visibleSize.width -pauseItem->getContentSize().width / 2,
                    origin.y + pauseItem->getContentSize().height / 2));

    auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    auto label = Label::createWithTTF("PAUSE", "fonts/Marker Felt.ttf", 96);
    label->setPosition(origin.x + _visibleSize.width/2,
                     origin.y + _visibleSize.height /2);
    this->addChild(label, 1);
    
    return true;
}

void PauseScene::exitPause(cocos2d::Ref* pSender){
    /*Pop the pause scene from the Scene stack.
     This will remove current scene.*/
    Director::getInstance()->popScene();
}
