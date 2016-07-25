#include "MoveTo.h"
#include "assert.h"
NS_HEHE2D_BEGIN

void MoveTo::step( float dt )
{
    timer_ += dt;
    if(timer_ < totalTime_)
    {
        float rate = timer_ / totalTime_;
        float x = src_.x + rate * (dst_.x - src_.x);
        float y = src_.y + rate * (dst_.y - src_.y);
        target_->setPos(Point(x, y));
    }
    else
    {
        target_->setPos(dst_);
    }
}

void MoveTo::setTarget( Node* v )
{
    assert(v);
    target_ = v;
    src_ = target_->getPos();
}

NS_HEHE2D_END