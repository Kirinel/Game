//
//  Player.cpp
//  helloworld
//
//  Created by 张智昊 on 04/06/2018.
//
//

#include "Player.hpp"
#include "lib.h"

Player::Player()
{
    m_iHP = PLAYER_HP;
    m_rHP = PLAYER_HP;
    m_shoot_angle = 0;
}

Player::~Player()
{
}
bool Player::init()
{
    playerSp = Sprite::create("HelloWorld.png");
    
    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,Event* event){
        // If a key already exists, do nothing as it will already have a time stamp
        // Otherwise, set's the timestamp to now
        if(keys.find(keyCode) == keys.end()){
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };
    
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        // remove the key.  std::map.erase() doesn't care if the key doesnt exist
        if(keyCode == EventKeyboard::KeyCode::KEY_SPACE)
            this->shoot();
        keys.erase(keyCode);
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,playerSp);
    this->setScale(0.25);
    this->addChild(playerSp);
    auto body = PhysicsBody::createCircle((playerSp->getContentSize().width) * 0.4f);
    body->getShape(0)->setFriction(0);
    body->getShape(0)->setRestitution(1.0f);
    body->setCategoryBitmask(1);    // 0001
    body->setCollisionBitmask(1);   // 0001
    body->setContactTestBitmask(1); // 0001
    this->setPhysicsBody(body);
    
    //add hp icon
    hpIcon = Hp_Icon::create();
    this->addChild(hpIcon, 10, ObjectTag_HP);
    ProgressTimer* pT = (ProgressTimer*)this->getChildByTag(ObjectTag_HP)->getChildByTag(ObjectTag_PT);
    pT->setPercentage(100);
    this->setTag(ObjectTag_Player);
    
    return true;
}

bool Player::isKeyPressed(EventKeyboard::KeyCode code) {
    // Check if the key is currently pressed by seeing it it's in the std::map keys
    // In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
    if(keys.find(code) != keys.end())
        return true;
    return false;
}

double Player::keyPressedDuration(EventKeyboard::KeyCode code) {
    if(!isKeyPressed(code))
        return 0;  // Not pressed, so no duration obviously
    
    // Return the amount of time that has elapsed between now and when the user
    // first started holding down the key in milliseconds
    // Obviously the start time is the value we hold in our std::map keys
    return std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::high_resolution_clock::now() - keys[code]).count();
}


std::map<cocos2d::EventKeyboard::KeyCode,std::chrono::high_resolution_clock::time_point> Player::keys;

void Player::moveToRight()
{
    //this->getPhysicsBody()->applyImpulse(Vect(50000, 0));
    this->getPhysicsBody()->setVelocity(Vec2(150, this->getPhysicsBody()->getVelocity().y));
}
void Player::moveToLeft()
{
    this->getPhysicsBody()->setVelocity(Vec2(-150, this->getPhysicsBody()->getVelocity().y));
}
void Player::shoot()
{
    auto bullet = Bullet::create();
    double weight = keyPressedDuration(EventKeyboard::KeyCode::KEY_SPACE);
    double angle = this->getshoot_angle();
    bullet->setPosition(Vec2(this->getPosition().x+BULLET_FIRE_DIS,this->getPosition().y+BULLET_FIRE_DIS));
    bullet->getPhysicsBody()->applyImpulse(Vect(7*weight, 20*weight));
    this->getParent()->addChild(bullet);
}

void Player::getAttack(int harm)
{
    this->setiHP(this->getiHP()-harm);
    ProgressTimer* pT = (ProgressTimer*)this->getChildByTag(ObjectTag_HP)->getChildByTag(ObjectTag_PT);
    int res = 100*this->getiHP()/this->getrHP();
    pT->setPercentage(res);
}


void Player::logic(float dt)
{
    // Register an update function that checks to see if the CTRL key is pressed
    // and if it is displays how long, otherwise tell the user to press it
    //Node::update(delta);
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        this->moveToRight();
    }
    else if(isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        this->moveToLeft();
    }
    else if(isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        
    }
    else if(isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        this->moveToLeft();
    }
    else if(isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)){
        ProgressTimer * pt = (ProgressTimer*)this->getParent()->getChildByTag(ObjectTag_SSI)->getChildByTag(ObjectTag_PT);
        pt->setVisible(1);
        if(keyPressedDuration(EventKeyboard::KeyCode::KEY_SPACE)>MAX_STRENGTH)
        {
            pt->setPercentage(100);
        }
        else{
            pt->setPercentage(100*keyPressedDuration(EventKeyboard::KeyCode::KEY_SPACE)/MAX_STRENGTH);
        }
    }
    else{
        auto velocity = this->getPhysicsBody()->getVelocity();
        this->getPhysicsBody()->setVelocity(Vec2(0, velocity.y));
        ProgressTimer * pt = (ProgressTimer*)this->getParent()->getChildByTag(ObjectTag_SSI)->getChildByTag(ObjectTag_PT);
        pt->setVisible(0);
    }
}