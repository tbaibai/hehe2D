#pragma once
#include "Node.h"
#include "Sprite.h"
class TestNode : public hehe2d::Node
{
public:
    TestNode(void);
    virtual ~TestNode(void);
    virtual void update(float dt);
private:
    hehe2d::Sprite* sp_;
    float speed_;
    float rotSpeed_;
};

