//
//  MainScene.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "MainScene.h"

USING_NS_CC;

const Vec2 GRAVITY_ACCELERATION = Vec2(0, -3);
const Vec2 IMPULSE_ACCELERATION = Vec2(0, 180);

Scene* MainScene::createScene()
{
    // 物理エンジンを有効にしたシーンを作成する
    auto scene = Scene::createWithPhysics();
    
    // 物理空間を取り出す
    auto world = scene->getPhysicsWorld();
    
    // 重力を設定する
    world->setGravity(GRAVITY_ACCELERATION);
    
    //#if COCOS2D_DEBUG > 1
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    //#endif
    // スピードを設定する
    world->setSpeed(6.0f);
    
    auto layer = MainScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    this->scheduleUpdate();
    
    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto parallaxNode = ParallaxNode::create();
    this->addChild(parallaxNode);
    
    auto layer = Stage::create();
    this->setStage(layer);
    
    auto mapWidth = layer->getTiledMap()->getContentSize().width;
    auto backgroundWidth = background->getContentSize().width;
    
    parallaxNode->addChild(background, 0, Vec2((backgroundWidth - winSize.width) / mapWidth, 0), Vec2::ZERO);
    this->setParallaxNode(parallaxNode);
    
    // 物体が衝突したことを検知するEventListener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact& contact) {
        
        auto otherShape = contact.getShapeA()->getBody() == _stage->getPlayer()->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
        auto body = otherShape->getBody();
        
        auto category = body->getCategoryBitmask();
        
        if (category & (int)Stage::TileType::ENEMY) {
            // ゲームオーバー
            auto explosition = ParticleExplosion::create();
            explosition->setPosition(_stage->getPlayer()->getPosition());
            _stage->addChild(explosition);
            this->onGameOver();
        } else if (category & (int)Stage::TileType::COIN) {
            // コイン
            body->getNode()->removeFromParent(); // コイン消す
            _coin += 1;
        }
        
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->addChild(layer);
    
    auto label = Label::createWithTTF(StringUtils::toString(_coin), "fonts/Marker Felt.ttf", 24);
    this->addChild(label);
    label->setPosition(Vec2(100, winSize.height - 30));
    label->enableShadow();
    this->setCoinLabel(label);
    
    // タッチしたときにタッチされているフラグをオンにする
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
        this->setIsPress(true);
        return true;
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
        this->setIsPress(false);
    };
    listener->onTouchCancelled = [this](Touch *touch, Event *event) {
        this->setIsPress(false);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    for (int i = 0; i < 10; ++i) {
        auto gear = Sprite3D::create("model/gear.obj");
        gear->setScale((rand() % 25) / 10.f);
        gear->setRotation3D(Vec3(0, 0, rand() % 90 - 45));
        auto rotate = RotateBy::create(10.0, Vec3(0, 360, 0));
        gear->runAction(RepeatForever::create(rotate));
        parallaxNode->addChild(gear, 2, Vec2(0.2, 0), Vec2(i * 200, 150));
    }
    auto ground = Sprite3D::create("model/ground.obj");
    ground->setScale(10);
    parallaxNode->addChild(ground, 2, Vec2(0.2, 0), Vec2(mapWidth / 2.0, 80));
    
    return true;
}

MainScene::MainScene() :
_isPress(false),
_coin(0),
_state(State::MAIN),
_stage(nullptr),
_parallaxNode(nullptr),
_coinLabel(nullptr)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_stage);
    CC_SAFE_RELEASE_NULL(_parallaxNode);
    CC_SAFE_RELEASE_NULL(_coinLabel);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void MainScene::update(float dt)
{
    // 背景をプレイヤーの位置によって動かす
    _parallaxNode->setPosition(_stage->getPlayer()->getPosition() * -1);
    
    if (_stage->getPlayer()->getPosition().x >= _stage->getTiledMap()->getContentSize().width) {
        if (_state == State::MAIN) {
            this->onClear();
        }
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    auto position = _stage->getPlayer()->getPosition();
    const auto margin = 50;
    if (position.y < -margin || position.y >= winSize.height + margin) {
        if (this->getState() == State::MAIN) {
            this->onGameOver();
        }
    }
    
    this->getCoinLabel()->setString(StringUtils::toString(_coin));
    
    // 画面がタップされている間
    if (this->getIsPress()) {
        // プレイヤーに上方向の推進力を与える
        _stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
}

void MainScene::onGameOver()
{
    
    this->setState(State::GAMEOVER);
    _stage->getPlayer()->removeFromParent();
    
    auto winSize = Director::getInstance()->getWinSize();
    auto label = Label::createWithSystemFont("もう一度", "Helvetica", 32);
    auto menuItem = MenuItemLabel::create(label, [](Ref *sender) {
        auto scene = MainScene::createScene();
        auto transition = TransitionFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    auto menu = Menu::create(menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(winSize.width / 2.0, winSize.height / 2.0);
    
}

void MainScene::onClear()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    this->setState(State::CLEAR);
    this->getEventDispatcher()->removeAllEventListeners();
    
    _stage->getPlayer()->getPhysicsBody()->setEnable(false);
    auto clearLabel = Label::createWithSystemFont("Clear!", "Helvetica", 64);
    clearLabel->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0 + 50));
    this->addChild(clearLabel);
    clearLabel->setColor(Color3B::RED);
    
    auto label = Label::createWithSystemFont("次のステージへ", "Helvetica", 32);
    auto menuItem = MenuItemLabel::create(label, [](Ref *sender) {
        auto scene = MainScene::createScene();
        auto transition = TransitionFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    auto menu = Menu::create(menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(winSize.width / 2.0, winSize.height / 2.0);
}