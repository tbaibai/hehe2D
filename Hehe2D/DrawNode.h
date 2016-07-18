#pragma once
#include "node.h"
NS_HEHE2D_BEGIN

class DrawNode :
    public Node
{
public:
    DrawNode(void);
    virtual ~DrawNode(void);
    virtual void draw();
};

NS_HEHE2D_END