//
//  TitleScene.h
//  KawazJet
//
//  Created by giginet on 2014/8/1.
//
//

#ifndef __KawazJet__TitleScene__
#define __KawazJet__TitleScene__

#include "cocos2d.h"

class TitleScene :public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    TitleScene();
    virtual ~TitleScene();
    
    bool init() override;
    void onEnterTransitionDidFinish() override;
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__KawazJet__TitleScene__) */
