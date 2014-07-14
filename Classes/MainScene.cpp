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
    
    //world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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
    
    auto layer = MapLayer::create();
    this->setMap(layer);
    
    auto mapWidth = layer->getTiledMap()->getContentSize().width;
    auto backgroundWidth = background->getContentSize().width;
    
    parallaxNode->addChild(background, 0, Vec2((backgroundWidth - winSize.width) / mapWidth, 0), Vec2::ZERO);
    this->setParallaxNode(parallaxNode);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact& contact) {
        
        auto otherShape = contact.getShapeA()->getBody() == _map->getPlayer()->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
        auto body = otherShape->getBody();
        
        auto category = body->getCategoryBitmask();
        
        if (category & (int)MapLayer::TileType::ENEMY) {
            // ゲームオーバー
            auto explosition = ParticleExplosion::create();
            explosition->setPosition(_map->getPlayer()->getPosition());
            _map->addChild(explosition);
            this->onGameOver();
        } else if (category & (int)MapLayer::TileType::COIN) {
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
    
    // タッチしたときに浮遊する処理を追加
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
    
    return true;
}

MainScene::MainScene() :
_isPress(false),
_coin(0),
_state(State::MAIN),
_map(nullptr),
_parallaxNode(nullptr),
_coinLabel(nullptr)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_map);
    CC_SAFE_RELEASE_NULL(_parallaxNode);
    CC_SAFE_RELEASE_NULL(_coinLabel);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void MainScene::update(float dt)
{
    _parallaxNode->setPosition(_map->getPlayer()->getPosition() * -1);
    
    if (_map->getPlayer()->getPosition().x >= _map->getTiledMap()->getContentSize().width) {
        if (_state == State::MAIN) {
            this->onClear();
        }
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    auto position = _map->getPlayer()->getPosition();
    const auto margin = 50;
    if (position.y < -margin || position.y >= winSize.height + margin) {
        if (this->getState() == State::MAIN) {
            this->onGameOver();
        }
    }
    
    this->getCoinLabel()->setString(StringUtils::toString(_coin));
    if (this->getIsPress()) {
        _map->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
}

void MainScene::onGameOver()
{
    
    this->setState(State::GAMEOVER);
    _map->getPlayer()->removeFromParent();
    
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
    
    _map->getPlayer()->setVelocity(Vec2::ZERO);
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