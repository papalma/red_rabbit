//
//  GameOverScene.cpp
//  TestGame
//
//  Created by Patricio Palma Contreras on 11/10/18.
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    return GameOverScene::create();
}

bool GameOverScene::init()
{
    if( !Scene::init() ) {
        return false;
    }

    _director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

    if( auto pauseItem = MenuItemImage::create("play.png",
            "play_pressed.png", CC_CALLBACK_1(GameOverScene::exitPause, this)) ) {
        pauseItem->setPosition(
                Vec2(origin.x + _visibleSize.width -pauseItem->getContentSize().width / 2,
                        origin.y + pauseItem->getContentSize().height / 2));

        auto menu = Menu::create(pauseItem, nullptr);
        menu->setPosition(Vec2());
        this->addChild(menu, 1);
    }

    if (auto label = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 32)) {
        label->setPosition(origin.x + _visibleSize.width/2,
                         origin.y + _visibleSize.height /2);
//        label->enableOutline(Color4B(255,0,0,100), 6);
//        label->enableGlow(Color4B(255, 0,0,100));
        label->enableShadow();
        this->addChild(label, 1);
    }

     if( auto bg = Sprite::create("background.png")) {
        bg->setPosition(
                origin.x + bg->getContentSize().width / 2,
                origin.y + bg->getContentSize().height /2);
         this->addChild(bg, -1);
     }
    
    return true;
}

void GameOverScene::exitPause(cocos2d::Ref* pSender){
    Director::getInstance()->replaceScene(
            TransitionFlipX::create(1.0f, HelloWorld::createScene()));
}
