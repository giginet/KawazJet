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
    
    auto layer = MapLayer::create();
    this->addChild(layer);
    this->setMap(layer);
    
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
    
    auto body = PhysicsBody::create();
    this->setPhysicsBody(body);
    
    return true;
}

MainScene::MainScene() : rotate(0)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_map);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void MainScene::update(float dt)
{
    rotate += 30 * dt;
    _map->setRotation(rotate);
    
    
    if (this->getIsPress()) {
        _map->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
}