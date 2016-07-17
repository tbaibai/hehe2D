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
{
    EsHelper::matrixLoadIdentity(transform_);
}


Node::~Node(void)
{
}

void Node::visit()
{
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
    children_.push_back(child);
}

bool Node::compare( Node* a, Node* b )
{
    return a->getZOrder() < b->getZOrder();
}

void Node::computeTranform()
{
    Size winSize = Renderer::instance()->getWinSize();
    Matrix tmp;
    EsHelper::matrixLoadIdentity(tmp);
    EsHelper::translate(tmp, pos_.x * 2 / winSize.w - 1.f, pos_.y * 2 / winSize.h - 1.f, 0.f);
    transform_ = tmp;
}

NS_HEHE2D_END