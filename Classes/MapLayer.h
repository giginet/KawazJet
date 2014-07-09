//
//  MapLayer.h
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#ifndef __KawazJet__MapLayer__
#define __KawazJet__MapLayer__

#include "cocos2d.h"
#include "Player.h"

class MapLayer :public cocos2d::Layer
{
CC_CONSTRUCTOR_ACCESS:
    bool init() override;
    
    MapLayer();
    virtual ~MapLayer();
    
public:

    void update(float dt) override;

    CREATE_FUNC(MapLayer);
    CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
};

#endif /* defined(__KawazJet__MapLayer__) */
