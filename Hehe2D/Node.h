#pragma once
#include "macros.h"
#include "PointSizeRect.h"
#include "EsHelper.h"
#include <vector>

NS_HEHE2D_BEGIN

class Action;
class Node
{
public:
    Node(void);
    virtual ~Node(void);
    virtual void visit();
    virtual void draw();
    virtual void update(float dt);
    virtual void setPos(const Point& v){pos_ = v;}
    virtual void setScale(float sx, float sy){scaleX_ = sx; scaleY_ = sy;}
    virtual float getRotation(){return angle_;}
    virtual void setRotation(float angle){angle_ = angle;}
    virtual const Point& getPos(){return pos_;}
    virtual void addChild(Node* child, int zOrder = 0);
    virtual Node* getParent(){return parent_;}
    virtual int getZOrder(){return zOrder_;}
    virtual void setZOrder(int v){zOrder_ = v;}
    virtual int getTag(){return tag_;}
    virtual void setTag(int v){tag_ = v;}
    virtual void computeTranform();
    Matrix& getTransform(){return transform_;}
    static bool compare(Node* a, Node* b);
    void runAction(Action* act);

protected:
    void _updateActions(float dt);

protected:
    Point pos_;
    float scaleX_;
    float scaleY_;
    float angle_;
    int zOrder_;
    int tag_;
    std::vector<Node*> children_;
    Node* parent_;

    Matrix transform_;

    std::vector<Action*> actions_;
};

NS_HEHE2D_END