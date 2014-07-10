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
            log("%d %d", x, y);
            auto sprite = terrainLayer->getTileAt(coordinate);
            if (sprite) {
                auto physicsBody = PhysicsBody::createBox(sprite->getContentSize());
                physicsBody->setGravityEnable(false);
                physicsBody->setCategoryBitmask((int)TileType::WALL);
                physicsBody->setContactTestBitmask((int)TileType::PLAYER);
                physicsBody->setCollisionBitmask((int)TileType::PLAYER);
                sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprite->setPhysicsBody(physicsBody);
            }
            
            auto object = objectLayer->getTileAt(coordinate);
            if (object) {
                auto physicsBody = PhysicsBody::createCircle(object->getContentSize().width / 2.0);
                physicsBody->setGravityEnable(false);
                physicsBody->setCategoryBitmask((int)TileType::ITEM);
                physicsBody->setCollisionBitmask(0);
                physicsBody->setContactTestBitmask((int)TileType::PLAYER);
                object->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                object->setPhysicsBody(physicsBody);
            }
        }
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [](PhysicsContact& contact) {
        log("hit");
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
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
    //_tiledMap->setPosition(_tiledMap->getPosition() - SCROLL_SPEED * dt);
    //auto point = this->convertToNodeSpace(Vec2(100, 0));
    //_player->setPosition(Vec2(point.x, _player->getPosition().y));
}