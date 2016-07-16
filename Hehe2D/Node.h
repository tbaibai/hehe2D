#pragma once
#include "macros.h"
#include "Point.h"
#include <vector>
NS_HEHE2D_BEGIN

class Node
{
public:
	Node(void);
	virtual ~Node(void);
	virtual void visit();
	virtual void draw();
	virtual void update(float dt);
	virtual void setPos(const Point& v){pos_ = v;}
	virtual const Point& getPos(){return pos_;}
	virtual void addChild(Node* child);
	virtual Node* getParent(){return parent_;}
	virtual int getZOrder(){return zOrder_;}
	virtual void setZOrder(int v){zOrder_ = v;}
	virtual int getTag(){return tag_;}
	virtual void setTag(int v){tag_ = v;}
    static bool compare(Node* a, Node* b);

private:
	Point pos_;
	int zOrder_;
	int tag_;
	std::vector<Node*> children_;
	Node* parent_;

};

NS_HEHE2D_END