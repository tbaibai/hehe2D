#include "stdio.h"
#include "malloc.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Renderer.h"
#include "Sprite.h"

using namespace hehe2d;
int main(int argc, char * argv[])
{
    Renderer::instance()->init(960, 640);

    Sprite* sp = new Sprite("picture_frame.png");
    Renderer::instance()->rootNode()->addChild(sp);
    sp->setPos(Point(400, 320));
    Renderer::instance()->run();

    return 0;
}