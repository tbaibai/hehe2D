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

    Sprite* sp = new Sprite("D:/GitProjs/game_dn_cocos2dx/game_client/Resources/images/share/picture_frame.png");
    Renderer::instance()->rootNode()->addChild(sp);

    Renderer::instance()->run();

    return 0;
}