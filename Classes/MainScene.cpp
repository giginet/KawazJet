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
    
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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
    
    this->scheduleUpdate();
    
    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto parallaxNode = ParallaxNode::create();
    this->addChild(parallaxNode);
    
    auto layer = MapLayer::create();
    this->setMap(layer);
    
    auto mapWidth = layer->getTiledMap()->getContentSize().width;
    auto backgroundWidth = background->getContentSize().width;
    
    parallaxNode->addChild(background, 0, Vec2((backgroundWidth - winSize.width) / (mapWidth - winSize.width), 0), Vec2::ZERO);
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
            _map->getPlayer()->removeFromParent();
            _map->addChild(explosition);
        } else if (category & (int)MapLayer::TileType::COIN) {
            // コイン
            body->getNode()->removeFromParent(); // コイン消す
        }
        
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->addChild(layer);
    
    return true;
}

MainScene::MainScene() :
_isPress(false),
_map(nullptr),
_parallaxNode(nullptr)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_map);
    CC_SAFE_RELEASE_NULL(_parallaxNode);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void MainScene::update(float dt)
{
    _parallaxNode->setPosition(_parallaxNode->getPosition() - _map->getPlayer()->getVelocity() * dt);
    if (this->getIsPress()) {
        _map->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
}