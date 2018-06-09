//
//  GameScene.hpp
//  helloworld
//
//  Created by 张智昊 on 04/06/2018.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "Player.hpp"
#include "Bullet.hpp"
#include "Strength_Saving_Icon.hpp"
#include "Enemy.hpp"

USING_NS_CC;

class GameScene : public Layer
{
public:
    GameScene();
    ~GameScene();
    static Scene* createScene();
    CREATE_FUNC(GameScene);
    virtual bool init();
    virtual void onExit() override;
    void logic(float dt);
    /* collision detection */
    bool onContactBegin(PhysicsContact& contact);
private:
    Player * m_player;
    bool collision_flag;
    CC_SYNTHESIZE(int, time_counter, tc);
};

#endif /* GameScene_hpp */
