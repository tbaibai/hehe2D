#pragma once

#include "macros.h"

NS_HEHE2D_BEGIN

struct Point
{
    float x;
    float y;

    Point() : x(0), y(0){}
    Point(float _x, float _y) : x(_x), y(_y){}
};

NS_HEHE2D_END