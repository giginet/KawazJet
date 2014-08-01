//
//  TitleScene.cpp
//  KawazJet
//
//  Created by giginet on 2014/8/1.
//
//

#include "TitleScene.h"

#include "TitleScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "AudioUtils.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

bool TitleScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    // 画面をタッチしたときにメイン画面へ遷移
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) {
        
        auto main = MainScene::createSceneWithStage(0);
        auto transition = TransitionCrossFade::create(0.5, main);
        
        Director::getInstance()->replaceScene(transition);
        
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    // BGMの再生
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(AudioUtils::getFileName("title").c_str(), true);
}