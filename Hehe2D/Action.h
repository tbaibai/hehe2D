#pragma once
#include "macros.h"
#include "Node.h"

NS_HEHE2D_BEGIN

class Action
{
public:
    Action(void) : target_(NULL), timer_(0.f), totalTime_(0){}
    virtual void setTarget(Node* v){target_ = v;}
    virtual ~Action(void){}

public:
    virtual void step(float dt) = 0;
    bool isDone(){return timer_ >= totalTime_;}
    float getSurplusTime(){return timer_ - totalTime_;}

protected:
    Node* target_;
    float timer_;
    float totalTime_;
};

NS_HEHE2D_END
