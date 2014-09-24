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
    
    auto winSize = Director::getInstance()->getWinSize();
    auto background = Sprite::create("title.png");
    background->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(background);
    
    auto start = Sprite::create("start.png");
    this->addChild(start);
    start->setPosition(Vec2(winSize.width / 2, winSize.height / 4));
    
    // 画面をタッチしたときにメイン画面へ遷移
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this, start](Touch* touch, Event* event) {
        start->setTexture(Director::getInstance()->getTextureCache()->addImage("start_pressed.png"));
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(AudioUtils::getFileName("start").c_str());
        this->getEventDispatcher()->removeAllEventListeners();
        
        this->runAction(Sequence::create(DelayTime::create(1.0),
                                         CallFunc::create([] {
            auto main = MainScene::createSceneWithStage(0);
            auto transition = TransitionCrossFade::create(0.5, main);
            
            Director::getInstance()->replaceScene(transition);
        }), NULL));
        
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