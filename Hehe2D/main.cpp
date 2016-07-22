#include "stdio.h"
#include "malloc.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Renderer.h"
#include "Sprite.h"
#include "DrawNode.h"
using namespace hehe2d;
int main(int argc, char * argv[])
{
    Renderer::instance()->init(960, 480);

    Sprite* sp = new Sprite("picture_frame.png");
    Renderer::instance()->rootNode()->addChild(sp);
    sp->setScale(2.f, 2.f);
    sp->setRotation(10);
    sp->setPos(Point(0, 0));

    /*Sprite* sp2 = new Sprite("picture_frame.png");
    Renderer::instance()->rootNode()->addChild(sp2);
    sp2->setPos(Point(240, 0));*/

    Sprite* sp3 = new Sprite("picture_frame.png");
    sp3->setPos(Point(96, 96));
    sp3->setScale(2.f, 2.f);
    sp->addChild(sp3);

    DrawNode* node = new DrawNode();
    node->setPos(Point(-300, -200));
    Renderer::instance()->rootNode()->addChild(node);
    Renderer::instance()->run();

    return 0;
}