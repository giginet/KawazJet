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
    
    enum class TileType {
        WALL = 1 << 0,
        PLAYER = 1 << 1,
        ENEMY = 1 << 2,
        ITEM = 1 << 3
    };

    void update(float dt) override;
    void onEnter() override;

    CREATE_FUNC(MapLayer);
    CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
};

#endif /* defined(__KawazJet__MapLayer__) */
