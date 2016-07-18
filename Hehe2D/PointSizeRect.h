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

struct Size
{
    float w;
    float h;

    Size() : w(0), h(0){}
    Size(float _w, float _h) : w(_w), h(_h){}
};
NS_HEHE2D_END