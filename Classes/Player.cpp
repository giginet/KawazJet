//
//  Player.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "Player.h"
#include "MapLayer.h"

USING_NS_CC;

bool Player::init()
{
    if (!Sprite::initWithFile("player.png")) {
        return false;
    }
    
    auto body = PhysicsBody::createBox(this->getContentSize());
    body->setRotationEnable(false);
    body->setCategoryBitmask((int)MapLayer::TileType::PLAYER);
    body->setCollisionBitmask((int)MapLayer::TileType::WALL);
    body->setContactTestBitmask(INT_MAX);
    this->setPhysicsBody(body);
    
    _velocity = Vec2(100, 0);
    
    return true;
}