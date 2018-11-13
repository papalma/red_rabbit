/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    static HelloWorld* createWithPhysics();

    virtual bool initWithPhysics();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
//    CREATE_FUNC(HelloWorld);
    void pauseCallback(cocos2d::Ref* pSender);
    void moveFinished(Node* sender);
    
    bool onCollision(cocos2d::PhysicsContact& contact);
    void setPhysicsBody(cocos2d::Sprite* sprite);

    // single touch support
    void initTouch();
    void movePlayerByTouch(cocos2d::Touch* touch, cocos2d::Event* event);
    void movePlayerIfPossible(float newX);

    // multitouch funcs
//    void initMultiTouch();
//    void moveByMultiTouch(const std::vector<cocos2d::Touch*>& touches,
//            cocos2d::Event* event);

    // accelerometer funcs
    void movePlayerByAccelerometer(cocos2d::Acceleration* acceleration,
            cocos2d::Event* event);
    void initAccelerometer();

    // android back button support
    // This is not needed for iOS, since home button is handled nicely by the
    // library.
//    void initBackButtonListener();
//    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void addBombs(float dt);

private:
    cocos2d::Sprite* _sprBomb;
    cocos2d::Sprite* _sprPlayer;
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
};

#endif // __HELLOWORLD_SCENE_H__
