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
    world->setGravity(Vec2(0,0));
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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    log("visibleSize %f %f", visibleSize.height, visibleSize.width);
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
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
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
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" logo
    if( auto sprite = Sprite::create("HelloWorld.png") ) {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

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

    if( auto sprBomb = Sprite::create("bomb.png") ) {
        sprBomb->setPosition(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height + sprBomb->getContentSize().height / 2);
                this->addChild(sprBomb,1);

        auto moveFinished =
                CallFuncN::create(CC_CALLBACK_1(HelloWorld::moveFinished, this));

        auto moveTo = MoveTo::create(
                2, Vec2(sprBomb->getPositionX(), 0 - sprBomb->getContentSize().height/2));
        auto sequence = Sequence::create(moveTo, moveFinished, nullptr);
        sprBomb->runAction(sequence);
    }

    if( auto sprPlayer = Sprite::create("player.png") ) {
        sprPlayer->setPosition(
                origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height * 0.21);
        this->addChild(sprPlayer, 0);

        Vector<SpriteFrame*> frames;
        Size playerSize = sprPlayer->getContentSize();
        frames.pushBack(SpriteFrame::create(
                "player.png", Rect(0, 0, playerSize.width, playerSize.height)));
        frames.pushBack(SpriteFrame::create(
                "player2.png", Rect(0, 0, playerSize.width, playerSize.height)));

        auto animation = Animation::createWithSpriteFrames( frames, 0.2f );
        auto animate = Animate::create( animation );
        sprPlayer->runAction(RepeatForever::create(animate));
    }

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

void HelloWorld::pauseCallback(cocos2d::Ref* pSender){
    _director->pushScene(
            TransitionFadeBL::create(1.0, PauseScene::createScene()));
}

void HelloWorld::moveFinished(Node* sender) {
        CCLOG("Move finished");
}
