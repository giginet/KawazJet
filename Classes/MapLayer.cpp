//
//  MapLayer.cpp
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#include "MapLayer.h"

USING_NS_CC;

const Vec2 SCROLL_SPEED = Vec2(100, 0);

MapLayer::MapLayer()
{
}

MapLayer::~MapLayer()
{
    CC_SAFE_RELEASE_NULL(_player);
    CC_SAFE_RELEASE_NULL(_tiledMap);
}

bool MapLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto map = TMXTiledMap::create("map/stage0.tmx");
    this->addChild(map);
    this->setTiledMap(map);
    
    // Playerの生成
    auto player = Player::create();
    player->setPosition(Vec2(100, 160));
    this->addChild(player);
    this->setPlayer(player);
    
    // updateを毎フレーム実行
    this->scheduleUpdate();
    
    return true;
}

void MapLayer::update(float dt)
{
    _tiledMap->setPosition(_tiledMap->getPosition() - SCROLL_SPEED * dt);
}