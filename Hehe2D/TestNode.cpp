#include "TestNode.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Action.h"
#include "MoveTo.h"
using namespace hehe2d;
TestNode::TestNode(void)
{
    Sprite* sp = new Sprite("picture_frame.png");
     Sprite* sp2 = new Sprite("picture_frame.png");
     sp2->setPos(Point(96, 96));
     sp2->setScale(0.5, 0.5);
     sp->addChild(sp2);
    Renderer::instance()->rootNode()->addChild(sp);
    sp->setScale(1.f, 1.f);
    sp->setPos(Point(0, 0));
    sp_ = sp;
    speed_ = 40;
    rotSpeed_ = 40;

    MoveTo* mt = new MoveTo(Point(480.f, 0.f), 5);
    sp->runAction(mt);
}


TestNode::~TestNode(void)
{
}

void TestNode::update(float dt)
{
    return;
    float px = sp_->getPos().x;
    float py = sp_->getPos().y;

    if(px + speed_ * dt > 100)
    {
        px = 100;
        speed_ = -speed_;
        rotSpeed_ = -rotSpeed_;
    }
    else if(px < -100)
    {
        speed_ = -speed_;
        rotSpeed_ = -rotSpeed_;
        px = -100;
    }
    else
    {
        px += speed_ * dt;
    }
    
    sp_->setPos(Point(px, py));

    sp_->setRotation(sp_->getRotation() + dt * rotSpeed_);
}
