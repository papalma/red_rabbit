#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PauseScene.h"
#include "GameOverScene.h"

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' "
            "in front of filenames in HelloWorldScene.cpp\n");
}

Scene* HelloWorld::createScene()
{
//    auto scene = HelloWorld::create();
    auto scene = HelloWorld::createWithPhysics();
    auto world = scene->getPhysicsWorld();
    world->setGravity(Vec2(0.0,0.0));
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    return scene;
}

HelloWorld* HelloWorld::createWithPhysics()
{
    HelloWorld *ret = new (std::nothrow) HelloWorld();
    if (ret && ret->initWithPhysics())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool HelloWorld::initWithPhysics()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
//    log("visibleSize %f %f", _visibleSize.height, _visibleSize.width);
//    log("_origin %f %f", _origin.x, _origin.y);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    auto closeItem = MenuItemImage::create(
            "pause.png",
            "pause_pressed.png",
            CC_CALLBACK_1(HelloWorld::pauseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = _origin.x + _visibleSize.width - closeItem->getContentSize().width/2;
        float y = _origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if( label ){
        // position the label on the center of the screen
        label->setPosition(Vec2(_origin.x + _visibleSize.width/2,
                                _origin.y + _visibleSize.height - label->getContentSize().height));
        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    if( auto bg = Sprite::create("background.png") ) {
        bg->setPosition(
                _origin.x + bg->getContentSize().width / 2,
                _origin.y + bg->getContentSize().height /2);
        this->addChild(bg, -1);
    }

    _sprBomb = Sprite::create("bomb.png");
    if( _sprBomb ) {
        _sprBomb->setPosition(
                _origin.x + _visibleSize.width / 2,
                _origin.y + _visibleSize.height + _sprBomb->getContentSize().height / 2);
        setPhysicsBody(_sprBomb);
        this->addChild(_sprBomb,1);

//        auto moveFinished =
//                CallFuncN::create(CC_CALLBACK_1(HelloWorld::moveFinished, this));
//
//        auto moveTo = MoveTo::create(
//                2, Vec2(_sprBomb->getPositionX(), 0 - _sprBomb->getContentSize().height/2));
//        auto sequence = Sequence::create(moveTo, moveFinished, nullptr);
//        _sprBomb->runAction(sequence);
        auto body = _sprBomb->getPhysicsBody();
        body->setVelocity(Vec2(0.0f, -90.0f));
//       body->setLinearDamping(0.1f);
    }

    _sprPlayer = Sprite::create("player.png");
    if( _sprPlayer ) {
        _sprPlayer->setPosition(
                _origin.x + _visibleSize.width / 2,
                _origin.y + _visibleSize.height * 0.21);
        setPhysicsBody(_sprPlayer);
        this->addChild(_sprPlayer, 0);

        Vector<SpriteFrame*> frames;
        Size playerSize = _sprPlayer->getContentSize();
        frames.pushBack(SpriteFrame::create(
                "player.png", Rect(0, 0, playerSize.width, playerSize.height)));
        frames.pushBack(SpriteFrame::create(
                "player2.png", Rect(0, 0, playerSize.width, playerSize.height)));

        auto animation = Animation::createWithSpriteFrames( frames, 0.2f );
        auto animate = Animate::create( animation );
        _sprPlayer->runAction(RepeatForever::create(animate));
    }



    // install a listener for collisions
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onCollision, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    initTouch();
//    initMultiTouch();
    initAccelerometer();

    schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addBombs), 3.0f);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the
    application  ,do not use Director::getInstance()->end() and exit(0) as given
    above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::pauseCallback(cocos2d::Ref* pSender) {
    _director->pushScene(
            TransitionFadeBL::create(1.0, PauseScene::createScene()));
}

void HelloWorld::moveFinished(Node* sender) {
        CCLOG("Move finished");
}

void HelloWorld::setPhysicsBody(cocos2d::Sprite* sprite) {
    auto body = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
    body->setContactTestBitmask(true);
    body->setDynamic(true);
    sprite->setPhysicsBody(body);
}

bool HelloWorld::onCollision(cocos2d::PhysicsContact& contact) {
    Director::getInstance()->replaceScene(
            TransitionFlipX::create(1.0f, GameOverScene::create()));
            return false;
}

void HelloWorld::initTouch() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        return true;
    };
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::movePlayerByTouch, this);
    listener->onTouchEnded = [=](Touch* touch, Event* event){};
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::movePlayerIfPossible(float newX) {
//    float sprHalfWidth = _sprPlayer->getBoundingBox().size.width / 2;
//    if(newX >= sprHalfWidth && newX < _visibleSize.width - sprHalfWidth) {
//    if( newX < _visibleSize.width - sprHalfWidth ) {
        _sprPlayer->setPositionX(newX);
//    }
}

void HelloWorld::movePlayerByTouch(Touch* touch, Event* event) {
    auto touchLocation = touch->getLocation();
//    cocos2d::log("loc: %f %f", touchLocation.x, touchLocation.y);
    if(_sprPlayer->getBoundingBox().containsPoint(touchLocation)){
        movePlayerIfPossible(touchLocation.x);
    }
}

// funcs for multi touch support
//void HelloWorld::initMultiTouch() {
//    auto listener = EventListenerTouchAllAtOnce::create();
//
//    listener->onTouchesBegan = [](const std::vector<Touch*>& touches, Event* event){ };
//    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::moveByMultiTouch,this);
//    listener->onTouchesEnded = [](const std::vector<Touch*>& touches, Event* event){ };
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
//}
//
//void HelloWorld::moveByMultiTouch(const std::vector<Touch*>& touches, Event* event) {
//    for( Touch* touch: touches ) {
//        Vec2 touchLocation = touch->getLocation();
//        if(_sprPlayer->getBoundingBox().containsPoint(touchLocation)){
//            movePlayerIfPossible(touchLocation.x);
//        } else if( _sprBomb->getBoundingBox().containsPoint(touchLocation)){
//            _sprBomb->setPosition(touchLocation);
//        }
//    }
//}

// funcs for accelerometer support
void HelloWorld::initAccelerometer() {
    // activate accelerometer.. what if there is no accel available?
    Device::setAccelerometerEnabled(true);

    auto listener = EventListenerAcceleration::create(
            CC_CALLBACK_2(HelloWorld::movePlayerByAccelerometer, this) );
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::movePlayerByAccelerometer(
        cocos2d::Acceleration* acceleration, cocos2d::Event* event) {
    int accelerationMult = 10;
    movePlayerIfPossible(_sprPlayer->getPositionX() + (acceleration->x * accelerationMult));
}

// funcs for android back button support
// for iOS this is handled nicely by the library.
//void HelloWorld::initBackButtonListener() {
//    auto listener = EventListenerKeyboard::create();
//    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) { };
//    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
//}
//
//void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
//    cocos2d::log("key pressed: %d", keyCode);
//     if(keyCode == EventKeyboard::KeyCode::KEY_BACK) {
//       Director::getInstance()->end();
//    }
//}

void HelloWorld::addBombs(float dt) {
    for(int i = 0 ; i < 3 ; i++) {
        if( auto bomb = Sprite::create("bomb.png")) {
            bomb->setPosition(
                    _origin.x + CCRANDOM_0_1() * _visibleSize.width,
                    _origin.y + _visibleSize.height + bomb->getContentSize().height/2);
            this->addChild(bomb,1);
            setPhysicsBody(bomb);
            bomb->getPhysicsBody()->setVelocity(Vect(0, ( (CCRANDOM_0_1() + 0.2f) * -250) ));
        }
    }
}
