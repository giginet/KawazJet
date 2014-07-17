//
//  Player.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "Player.h"
#include "Stage.h"

const int WIDTH = 35;
const int HEIGHT = 45;
const int ACCELERATION_LIMIT = 40;

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
    body->setCategoryBitmask((int)Stage::TileType::PLAYER);
    body->setCollisionBitmask((int)Stage::TileType::WALL);
    body->setContactTestBitmask(INT_MAX);
    this->setPhysicsBody(body);
    
    _acceleration = Vec2(200, 0);
    
    this->scheduleUpdate();
    
    return true;
}

void Player::update(float dt)
{
    this->getPhysicsBody()->applyImpulse(_acceleration);
    auto v = this->getPhysicsBody()->getVelocity();
    if (v.x > ACCELERATION_LIMIT) {
        v.x = ACCELERATION_LIMIT;
        this->getPhysicsBody()->setVelocity(v);
    }
}