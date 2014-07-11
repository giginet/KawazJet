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
    
    auto body = PhysicsBody::createCircle(this->getContentSize().width / 2.0);
    body->setRotationEnable(false);
    body->setCategoryBitmask((int)MapLayer::TileType::PLAYER);
    body->setCollisionBitmask((int)MapLayer::TileType::WALL);
    body->setContactTestBitmask(INT_MAX);
    this->setPhysicsBody(body);
    
    _velocity = Vec2(200, 0);
    
    return true;
}