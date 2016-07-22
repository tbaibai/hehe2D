#include "Node.h"
#include "assert.h"
#include "Renderer.h"
#include "PointSizeRect.h"
#include <algorithm>

using namespace std;

NS_HEHE2D_BEGIN
    Node::Node(void)
    : parent_(NULL)
    , tag_(0)
    , zOrder_(0)
    , scaleX_(1.f)
    , scaleY_(1.f)
    , angle_(0.f)
{
    EsHelper::matrixLoadIdentity(transform_);
}


Node::~Node(void)
{
}

void Node::visit()
{
    update(Renderer::instance()->getLastFrameTime());
    computeTranform();

    stable_sort(children_.begin(), children_.end(), &Node::compare);
    vector<Node*>::iterator it = children_.begin();
    while(it != children_.end() && (*it)->getZOrder() < 0)
    {
        (*it)->visit();
        ++it;
    }
    draw();
    while(it != children_.end())
    {
        (*it)->visit();
        ++it;
    }
}

void Node::draw()
{

}

void Node::update( float dt )
{
    return;
}

void Node::addChild( Node* child, int zOrder/*=0*/)
{
    assert(child && child->getParent() == NULL);
    child->setZOrder(zOrder);
    child->parent_ = this;
    children_.push_back(child);
}

bool Node::compare( Node* a, Node* b )
{
    return a->getZOrder() < b->getZOrder();
}

void Node::computeTranform()
{
    Size winSize = Renderer::instance()->getWinSize();

    EsHelper::matrixLoadIdentity(transform_);

    //计算自己的R*S*T，再右乘父节点的transform， 最后投影变换

    EsHelper::translate(transform_, pos_.x, pos_.y, 0.f);
    EsHelper::scale(transform_, scaleX_, scaleY_, 1.f);
    EsHelper::rotate(transform_, angle_);

    if(parent_ == nullptr)//root
    {
        Matrix projMat;
        EsHelper::matrixLoadIdentity(projMat);
        EsHelper::scale(projMat, 2.f / winSize.w, 2.f / winSize.h, 1.f);//投影
        EsHelper::matrixMultiply(transform_, transform_, projMat);
    }
    else
    {
        EsHelper::matrixMultiply(transform_, transform_, parent_->getTransform());
    }
}

NS_HEHE2D_END