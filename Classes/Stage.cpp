//
//  Stage.cpp
//  KawazJet
//
//  Created by giginet on 7/9/14.
//
//

#include "Stage.h"

USING_NS_CC;

Stage::Stage() :
_tiledMap(nullptr),
_player(nullptr)
{
}

Stage::~Stage()
{
    CC_SAFE_RELEASE_NULL(_tiledMap);
    CC_SAFE_RELEASE_NULL(_player);
}

bool Stage::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto map = TMXTiledMap::create("map/stage0.tmx");
    this->addChild(map);
    this->setTiledMap(map);
    
    // 地形レイヤーを取得する
    auto terrainLayer = map->getLayer("Terrain");
    // オブジェクトレイヤーを取得する
    auto objectLayer = map->getLayer("Object");
    
    // マップのサイズ
    auto mapSize = map->getMapSize();
    for (int x = 0; x < mapSize.width; ++x) {
        for (int y = 0; y < mapSize.height; ++y) {
            auto coordinate = Vec2(x, y);
            
            this->addPhysicsBody(terrainLayer, coordinate);
            this->addPhysicsBody(objectLayer, coordinate);
            
        }
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    // Playerの生成
    auto player = Player::create();
    player->setPosition(Vec2(100, 160));
    this->addChild(player);
    this->setPlayer(player);
    
    this->runAction(Follow::create(player, Rect(0, 0, _tiledMap->getContentSize().width, winSize.height)));
    
    this->scheduleUpdate();
    
    return true;
}

void Stage::update(float dt)
{
}

Sprite* Stage::addPhysicsBody(cocos2d::TMXLayer *layer, cocos2d::Vec2 &coordinate)
{
    // タイル1枚の大きさを取り出す
    auto tileSize = _tiledMap->getTileSize() / 2.0;
    
    // タイルのスプライトを取り出す
    auto sprite = layer->getTileAt(coordinate);
    if (sprite) {
        // タイルのIDを取り出す
        auto gid = layer->getTileGIDAt(coordinate);
        // タイルのプロパティをmapで取り出す
        auto properties = _tiledMap->getPropertiesForGID(gid).asValueMap();
        // プロパティの中からcategoryの値をintとして取り出す
        auto category = properties.at("category").asInt();
        
        auto material = PhysicsMaterial();
        material.friction = 0;
        material.restitution = 0.5;
        
        // 剛体を設置する
        auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), material);
        // 剛体を固定する
        physicsBody->setDynamic(false);
        // 剛体にカテゴリをセットする
        physicsBody->setCategoryBitmask(category);
        // 剛体と当たり判定を取るカテゴリを指定する
        physicsBody->setContactTestBitmask((int)TileType::PLAYER);
        // 剛体と衝突判定を取るカテゴリを指定する
        physicsBody->setCollisionBitmask((int)TileType::PLAYER);
        
        // アニメーションを付ける
        // プロパティにanimationの値があったら
        if (!properties["animation"].isNull()) {
            auto animationSprite = properties["animation"].asString();
            auto animationCount = properties["animationCount"].asInt();
        
            sprite->removeFromParent();
            this->addChild(sprite);
            
            Vector<SpriteFrame *> frames;
            for (int i = 0; i < animationCount; ++i) {
                auto frame = SpriteFrame::create(animationSprite, Rect(tileSize.width * i, 0, tileSize.width, tileSize.height));
                frames.pushBack(frame);
            }
            auto animation = Animation::createWithSpriteFrames(frames);
            animation->setDelayPerUnit(0.15);
            sprite->runAction(RepeatForever::create(Animate::create(animation)));
        }
        
        sprite->setPhysicsBody(physicsBody);
        
        return sprite;
    }
    return nullptr;
}