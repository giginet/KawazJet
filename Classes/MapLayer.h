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
    
private:
    cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate);
    
public:
    
    enum class TileType {
        WALL = 1 << 0,  // 1
        PLAYER = 1 << 1, // 2
        ENEMY = 1 << 2, // 4
        COIN = 1 << 3 // 8
    };

    void update(float dt) override;
    void onEnter() override;

    CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
    CREATE_FUNC(MapLayer);
};

#endif /* defined(__KawazJet__MapLayer__) */
