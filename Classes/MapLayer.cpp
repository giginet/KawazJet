//
//  MapLayer.cpp
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#include "MapLayer.h"

USING_NS_CC;

MapLayer::MapLayer() :
_tiledMap(nullptr),
_player(nullptr)
{
}

MapLayer::~MapLayer()
{
    CC_SAFE_RELEASE_NULL(_tiledMap);
    CC_SAFE_RELEASE_NULL(_player);
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
    
    auto terrainLayer = map->getLayer("Terrain");
    auto objectLayer = map->getLayer("Object");
    
    // マップのサイズ
    auto mapSize = map->getMapSize();
    for (int x = 0; x < mapSize.width; ++x) {
        for (int y = 0; y < mapSize.height; ++y) {
            auto coordinate = Vec2(x, y);
            
            this->addPhysicsBody(terrainLayer, coordinate);
            this->addPhysicsBody(objectLayer, coordinate);
            
        }
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    // Playerの生成
    auto player = Player::create();
    player->setPosition(Vec2(100, 160));
    this->addChild(player);
    this->setPlayer(player);
    
    this->runAction(Follow::create(player, Rect(0, 0, _tiledMap->getContentSize().width, winSize.height)));
    
    return true;
}

void MapLayer::onEnter()
{
    Layer::onEnter();
    // updateを毎フレーム実行
    this->scheduleUpdate();
}

void MapLayer::update(float dt)
{
    _player->setPosition(_player->getPosition() + _player->getVelocity() * dt);
}

Sprite* MapLayer::addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2 &coordinate)
{
    auto sprite = layer->getTileAt(coordinate);
    if (sprite) {
        auto gid = layer->getTileGIDAt(coordinate);
        auto properties = _tiledMap->getPropertiesForGID(gid).asValueMap();
        auto category = properties.at("category").asInt();
        
        auto physicsBody = PhysicsBody::createBox(sprite->getContentSize());
        physicsBody->setGravityEnable(false);
        physicsBody->setCategoryBitmask(category);
        physicsBody->setContactTestBitmask((int)TileType::PLAYER);
        physicsBody->setCollisionBitmask((int)TileType::PLAYER);
        physicsBody->setDynamic(false);
        
        sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprite->setPhysicsBody(physicsBody);
        
        return sprite;
    }
    return nullptr;
}