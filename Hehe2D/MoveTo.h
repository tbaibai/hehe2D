#pragma once
#include "action.h"
#include "PointSizeRect.h"
NS_HEHE2D_BEGIN

class MoveTo :
    public Action
{
public:
    MoveTo(Point& dst, float t) : dst_(dst){ totalTime_ = t;}
    virtual ~MoveTo(void){}
    virtual void step(float dt);
    virtual void setTarget(Node* v);
protected:
    Point dst_;
    Point src_;

};

NS_HEHE2D_END