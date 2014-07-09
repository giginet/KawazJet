//
//  MainScene.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "MainScene.h"

USING_NS_CC;

const Vec2 GRAVITY_ACCELERATION = Vec2(0, -9.8);

Scene* MainScene::createScene()
{
    // 物理エンジンを有効にしたシーンを作成する
    auto scene = Scene::createWithPhysics();
    
    // 物理空間を取り出す
    auto world = scene->getPhysicsWorld();
    
    // 重力を設定する
    world->setGravity(GRAVITY_ACCELERATION);
    
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
    
    auto player = Sprite::create("player.png");
    
    auto body = PhysicsBody::createBox(player->getContentSize());
    player->setPhysicsBody(body);
    
    player->setPosition(Vec2(100, 160));
    
    this->addChild(player);
    
    return true;
}

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::onEnterTransitionDidFinish()
{
}