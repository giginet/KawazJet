//
//  Player.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "Player.h"

USING_NS_CC;

bool Player::init()
{
    if (!Sprite::initWithFile("player.png")) {
        return false;
    }
    
    auto body = PhysicsBody::createBox(this->getContentSize());
    body->setRotationEnable(false);
    this->setPhysicsBody(body);
    
    return true;
}