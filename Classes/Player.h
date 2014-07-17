//
//  Player.h
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#ifndef __KawazJet__Player__
#define __KawazJet__Player__

#include <iostream>
#include "cocos2d.h"

class Player :public cocos2d::Sprite
{
public:
    bool init() override;
    
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2, _velocity, Velocity);
    CREATE_FUNC(Player);
};

#endif /* defined(__KawazJet__Player__) */
