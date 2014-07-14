//
//  Player.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "Player.h"
#include "MapLayer.h"

const int WIDTH = 35;
const int HEIGHT = 45;

USING_NS_CC;

bool Player::init()
{
    if (!Sprite::initWithFile("player.png", Rect(0, 0, WIDTH, HEIGHT))) {
        return false;
    }
    
    Vector<SpriteFrame *> frames;
    for (int i = 0; i < 4; ++i) {
        auto frame = SpriteFrame::create("player.png", Rect(WIDTH * i, 0, WIDTH, HEIGHT));
        frames.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.05);
    this->runAction(RepeatForever::create(Animate::create(animation)));
    
    auto body = PhysicsBody::createCircle(this->getContentSize().width / 2.0);
    body->setRotationEnable(false);
    body->setCategoryBitmask((int)MapLayer::TileType::PLAYER);
    body->setCollisionBitmask((int)MapLayer::TileType::WALL);
    body->setContactTestBitmask(INT_MAX);
    this->setPhysicsBody(body);
    
    _velocity = Vec2(200, 0);
    
    return true;
}