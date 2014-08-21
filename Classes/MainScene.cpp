//
//  MainScene.cpp
//  KawazJet
//
//  Created by giginet on 2014/7/9.
//
//

#include "MainScene.h"
#include "AudioUtils.h"

USING_NS_CC;

/// ステージ数
const int STAGE_COUNT = 5;
const Vec2 GRAVITY_ACCELERATION = Vec2(0, -10);
const Vec2 IMPULSE_ACCELERATION = Vec2(0, 2000);

Scene* MainScene::createSceneWithStage(int level)
{
    // 物理エンジンを有効にしたシーンを作成する
    auto scene = Scene::createWithPhysics();
    
    // 物理空間を取り出す
    auto world = scene->getPhysicsWorld();
    
    // 重力を設定する
    world->setGravity(GRAVITY_ACCELERATION);
    
    //#if COCOS2D_DEBUG > 1
    //world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    //#endif
    // スピードを設定する
    world->setSpeed(6.0f);
    
    auto layer = new MainScene();
    if (layer && layer->initWithLevel(level)) {
        layer->autorelease();
    } else {
        CC_SAFE_RELEASE_NULL(layer);
    }
    
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    return this->initWithLevel(0);
}

bool MainScene::initWithLevel(int level)
{
    if (!Layer::init()) {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    this->scheduleUpdate();
    
    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto parallaxNode = ParallaxNode::create();
    this->addChild(parallaxNode);
    
    auto stage = Stage::createWithLevel(level);
    this->setStage(stage);
    
    auto mapWidth = stage->getTiledMap()->getContentSize().width;
    auto backgroundWidth = background->getContentSize().width;
    
    parallaxNode->addChild(background, 0, Vec2((backgroundWidth - winSize.width) / mapWidth, 0), Vec2::ZERO);
    this->setParallaxNode(parallaxNode);
    
    // 物体が衝突したことを検知するEventListener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact& contact) {
        
        auto otherShape = contact.getShapeA()->getBody() == _stage->getPlayer()->getPhysicsBody() ? contact.getShapeB() : contact.getShapeA();
        auto body = otherShape->getBody();
        
        auto category = body->getCategoryBitmask();
        auto layer = dynamic_cast<TMXLayer *>(body->getNode()->getParent());
        
        if (category & static_cast<int>(Stage::TileType::ENEMY)) {
            // ゲームオーバー
            this->onGameOver();
        } else if (category & (int)Stage::TileType::COIN) {
            // コイン
            layer->removeChild(body->getNode(), true);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(AudioUtils::getFileName("coin").c_str());
            _coin += 1;
        } else if (category & static_cast<int>(Stage::TileType::ITEN)) {
            // アイテム
            layer->removeChild(body->getNode(), true);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(AudioUtils::getFileName("food").c_str());
            this->onGetItem(body->getNode());
        }
        
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->addChild(stage);
    
    // タッチしたときにタッチされているフラグをオンにする
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
        this->setIsPress(true);
        return true;
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
        this->setIsPress(false);
    };
    listener->onTouchCancelled = [this](Touch *touch, Event *event) {
        this->setIsPress(false);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    for (int i = 0; i < 10; ++i) {
        auto wrapper = Node::create();
        auto gear = Sprite3D::create("model/gear.obj");
        gear->setScale((rand() % 20) / 10.f);
        wrapper->setRotation3D(Vec3(rand() % 45, 0, rand() % 90 - 45));
        auto rotate = RotateBy::create(rand() % 30, Vec3(0, 360, 0));
        gear->runAction(RepeatForever::create(rotate));
        wrapper->addChild(gear);
        parallaxNode->addChild(wrapper, 2, Vec2(0.2, 0), Vec2(i * 200, 160));
    }
    
    auto ground = Sprite3D::create("model/ground.obj");
    ground->setScale(15);
    parallaxNode->addChild(ground, 2, Vec2(0.5, 0), Vec2(mapWidth / 2.0, 80));
    
    // ステージ番号の表示
    auto stageBackground = Sprite::create("stage_ui.png");
    stageBackground->setPosition(Vec2(stageBackground->getContentSize().width / 2,
                                      winSize.height - stageBackground->getContentSize().height / 2.0));
    this->addChild(stageBackground);
    
    auto stageLabel = Label::createWithCharMap("numbers.png", 16, 18, '0');
    stageLabel->setString(StringUtils::format("%d", _stage->getLevel() + 1));
    stageLabel->setPosition(Vec2(60, winSize.height - 22));
    this->addChild(stageLabel);
    
    // 制限時間を表示
    auto secondLabel = Label::createWithCharMap("numbers.png", 16, 18, '0');
    secondLabel->setPosition(Vec2(300, winSize.height - 10));
    secondLabel->enableShadow();
    this->addChild(secondLabel);
    this->setSecondLabel(secondLabel);
    
    // コインの枚数の表示
    auto coin = Sprite::create("coin.png");
    coin->setPosition(Vec2(160, winSize.height - 15));
    this->addChild(coin);
    
    auto label = Label::createWithCharMap("numbers.png", 16, 18, '0');
    this->addChild(label);
    label->setPosition(Vec2(200, winSize.height - 10));
    label->enableShadow();
    this->setCoinLabel(label);
    
    
    // 取得したアイテムの数を表示
    const int maxItemCount = 3;
    for (int i = 0; i < maxItemCount; ++i) {
        auto sprite = Sprite::create("item.png");
        sprite->setPosition(Vec2(winSize.width - 70 + i * 20, winSize.height - 15));
        this->addChild(sprite);
        _items.pushBack(sprite);
        sprite->setColor(Color3B::BLACK);
    }
    
    return true;
}

MainScene::MainScene()
: _isPress(false)
, _coin(0)
, _itemCount(0)
, _second(0)
, _state(State::MAIN)
, _stage(nullptr)
, _parallaxNode(nullptr)
, _coinLabel(nullptr)
, _stageLabel(nullptr)
, _secondLabel(nullptr)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_stage);
    CC_SAFE_RELEASE_NULL(_parallaxNode);
    CC_SAFE_RELEASE_NULL(_coinLabel);
    CC_SAFE_RELEASE_NULL(_stageLabel);
    CC_SAFE_RELEASE_NULL(_secondLabel);
}

void MainScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(AudioUtils::getFileName("main").c_str(), true);
    // GO演出
    
    auto winSize = Director::getInstance()->getWinSize();
    auto go = Sprite::create("go.png");
    go->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(go);
    
    go->setScale(0);
    go->runAction(Sequence::create(ScaleTo::create(0.1, 1.0),
                                   DelayTime::create(0.5),
                                   ScaleTo::create(0.1, 0),
                                   NULL));
}

void MainScene::update(float dt)
{
    // 背景をプレイヤーの位置によって動かす
    _parallaxNode->setPosition(_stage->getPlayer()->getPosition() * -1);
    
    // クリア判定
    if (_stage->getPlayer()->getPosition().x >= _stage->getTiledMap()->getContentSize().width * _stage->getTiledMap()->getScale()) {
        if (_state == State::MAIN) {
            this->onClear();
        }
    }
    
    // 画面外からはみ出したとき、ゲームオーバー判定
    auto winSize = Director::getInstance()->getWinSize();
    auto position = _stage->getPlayer()->getPosition();
    const auto margin = 100;
    if (position.y < -margin || position.y >= winSize.height + margin) {
        if (this->getState() == State::MAIN) {
            this->onGameOver();
        }
    }
    
    // コインの枚数の更新
    this->getCoinLabel()->setString(StringUtils::toString(_coin));
    
    // 画面がタップされている間
    if (this->getIsPress()) {
        // プレイヤーに上方向の推進力を与える
        _stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
    }
    
    if (_state == State::MAIN) {
        _second += dt;
        this->updateSecond();
    }
}

void MainScene::onGameOver()
{
    this->setState(State::GAMEOVER);
    _stage->getPlayer()->removeFromParent();
    
    auto winSize = Director::getInstance()->getWinSize();
    int currentStage = _stage->getLevel();
    
    auto gameover = Sprite::create("gameover.png");
    gameover->setPosition(Vec2(winSize.width / 2.0, winSize.height / 1.5));
    this->addChild(gameover);
    
    auto menuItem = MenuItemImage::create("replay.png", "replay_pressed.png", [currentStage](Ref *sender) {
        auto scene = MainScene::createSceneWithStage(currentStage);
        auto transition = TransitionFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    auto menu = Menu::create(menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(winSize.width / 2.0, winSize.height / 3);
    
    // パーティクル表示
    auto explosition = ParticleExplosion::create();
    explosition->setPosition(_stage->getPlayer()->getPosition());
    _stage->addChild(explosition);
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(AudioUtils::getFileName("explode").c_str());
}

void MainScene::onClear()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    this->setState(State::CLEAR);
    this->getEventDispatcher()->removeAllEventListeners();
    
    _stage->getPlayer()->getPhysicsBody()->setEnable(false);
    auto clear = Sprite::create("clear.png");
    clear->setPosition(Vec2(winSize.width / 2.0, winSize.height / 1.5));
    this->addChild(clear);
    
    int nextStage = (_stage->getLevel() + 1) % STAGE_COUNT;
    
    auto menuItem = MenuItemImage::create("next.png", "next_pressed.png", [nextStage](Ref *sender) {
        auto scene = MainScene::createSceneWithStage(nextStage);
        auto transition = TransitionFade::create(1.0, scene);
        Director::getInstance()->replaceScene(transition);
    });
    auto menu = Menu::create(menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(winSize.width / 2.0, winSize.height / 3.0);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(AudioUtils::getFileName("clear").c_str());
}

void MainScene::onGetItem(cocos2d::Node * item)
{
    _itemCount += 1;
    for (int i = 0; i < _itemCount; ++i) {
        _items.at(i)->setColor(Color3B::WHITE);
    }
}

void MainScene::updateSecond()
{
    int sec = floor(_second);
    int milisec = floor((_second - sec) * 100);
    auto string = StringUtils::format("%03d:%02d", sec, milisec);
    _secondLabel->setString(string);
}
