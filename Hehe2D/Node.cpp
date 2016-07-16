#include "Node.h"
#include "assert.h"
#include <algorithm>

using namespace std;

NS_HEHE2D_BEGIN
Node::Node(void)
: parent_(NULL)
, tag_(0)
, zOrder_(0)
{
}


Node::~Node(void)
{
}

void Node::visit()
{
	stable_sort(children_.begin(), children_.end(), &Node::compare);
	vector<Node*>::iterator it = children_.begin();
	while((*it)->getZOrder() < 0 && it != children_.end())
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

void Node::addChild( Node* child )
{
	assert(child && child->getParent() == NULL);
	children_.push_back(child);
}

bool Node::compare( Node* a, Node* b )
{
    return a->getZOrder() < b->getZOrder();
}

NS_HEHE2D_END