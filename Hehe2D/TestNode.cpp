#include "TestNode.h"
#include "Sprite.h"
#include "Renderer.h"
using namespace hehe2d;
TestNode::TestNode(void)
{
    Sprite* sp = new Sprite("picture_frame.png");
    Renderer::instance()->rootNode()->addChild(sp);
    sp->setScale(2.f, 2.f);
    sp->setPos(Point(0, 0));
    sp_ = sp;
    speed_ = 40;
    rotSpeed_ = 40;
}


TestNode::~TestNode(void)
{
}

void TestNode::update(float dt)
{
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
