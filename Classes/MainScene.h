//
//  MainScene.h
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#ifndef __KawazJet__MainScene__
#define __KawazJet__MainScene__

#include <iostream>
#include "cocos2d.h"
#include "Stage.h"

class MainScene :public cocos2d::Layer
{
public:
    
    enum class State {
        READY,
        MAIN,
        CLEAR,
        GAMEOVER
    };
    
    static cocos2d::Scene* createSceneWithStage(int stageNumber);
    
    void onEnterTransitionDidFinish() override;
    
    MainScene();
    virtual ~MainScene();
    bool init() override;
    bool initWithStage(int stageNumber);
    void update(float dt) override;
    void updateSecond();
    
    /// 画面がタップされているかどうか
    CC_SYNTHESIZE(bool, _isPress, IsPress);
    /// 現在の取得コイン数
    CC_SYNTHESIZE(int, _coin, Coin);
    /// 現在のアイテム取得数
    CC_SYNTHESIZE(int, _itemCount, ItemCount);
    /// 現在のゲーム状態
    CC_SYNTHESIZE(State, _state, State);
    /// 現在の経過時間（秒）
    CC_SYNTHESIZE(float, _second, Second);
    /// アイテム表示用のスプライト一覧
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<cocos2d::Sprite *>, _items, Items);
    /// ステージ
    CC_SYNTHESIZE_RETAIN(Stage *, _stage, Stage);
    /// コイン枚数表示用のラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _coinLabel, CoinLabel);
    /// ステージ番号表示用のラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _stageLabel, StageLabel);
    /// 時間表示用のラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);
    /// 背景用ParallaxNode
    CC_SYNTHESIZE_RETAIN(cocos2d::ParallaxNode *, _parallaxNode, ParallaxNode);
    CREATE_FUNC(MainScene);
    
private:
    void onGameOver();
    void onClear();
    void onGetItem(cocos2d::Node * item);
};

#endif /* defined(__KawazJet__MainScene__) */
