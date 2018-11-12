#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PauseScene.h"

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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    log("visibleSize %f %f", _visibleSize.height, _visibleSize.width);
    log("origin %f %f", origin.x, origin.y);

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
        float x = origin.x + _visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
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
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + _visibleSize.width/2,
                                origin.y + _visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" logo
    if( auto sprite = Sprite::create("HelloWorld.png") ) {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(_visibleSize.width/2 + origin.x, _visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);

        sprite->runAction(FadeOut::create(1.0));
    }

    if( auto bg = Sprite::create("background.png") ) {
        bg->setPosition(
                origin.x + bg->getContentSize().width / 2,
                origin.y + bg->getContentSize().height /2);
        this->addChild(bg, -1);
    }

    _sprBomb = Sprite::create("bomb.png");
    if( _sprBomb ) {
        _sprBomb->setPosition(
                origin.x + _visibleSize.width / 2,
                origin.y + _visibleSize.height + _sprBomb->getContentSize().height / 2);
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
                origin.x + _visibleSize.width / 2,
                origin.y + _visibleSize.height * 0.21);
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
//    _sprBomb->setVisible(false);
    auto body = _sprBomb ->getPhysicsBody();
    body->setVelocity(Vec2());
    body->applyTorque(100900.5f);
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
