//
//  Stage.h
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#ifndef __KawazJet__Stage__
#define __KawazJet__Stage__

#include "cocos2d.h"
#include "Player.h"

class Stage :public cocos2d::Layer
{
public:
    
    enum class TileType {
        WALL = 1 << 0,  // 1
        PLAYER = 1 << 1, // 2
        ENEMY = 1 << 2, // 4
        COIN = 1 << 3 // 8
    };
    
    bool init() override;
    
    Stage();
    virtual ~Stage();

    void update(float dt) override;

    CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap *, _tiledMap, TiledMap);
    CC_SYNTHESIZE_RETAIN(Player *, _player, Player);
    CREATE_FUNC(Stage);
private:
    /** 指定のレイヤーの特定位置のタイルに剛体を設置します
     *  指定座標にタイルがなかった場合はnullptrを返します
     *  @param layer 対象のレイヤー
     *  @param coordinate 対象タイルのあるマップ上の座標
     *  @return タイルのスプライト、またはnullptr
     */
    cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2& coordinate);
    
};

#endif /* defined(__KawazJet__Stage__) */
