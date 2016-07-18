#include "DrawNode.h"
#include "Renderer.h"
NS_HEHE2D_BEGIN

DrawNode::DrawNode(void)
{
}


DrawNode::~DrawNode(void)
{
}

void DrawNode::draw()
{
    Renderer::instance()->addShaderProgram("matrixTest", "matTest.vs", "matTest.fs");
    GLuint obj = Renderer::instance()->useShaderProgram("matrixTest");

    GLfloat mat[] = {
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0
    };

    GLuint matLoc = glGetUniformLocation(obj, "u_m");
    glUniformMatrix4fv(matLoc, 1,false, mat);

    GLfloat vertData[] = {
        0.5f, 0.5f, 0.f,
        -0.5f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
    };
    GLuint pos = glGetAttribLocation ( Renderer::instance()->getShaderProgram("matrixTest"), "a_position" );
    glVertexAttribPointer ( pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertData );
    glEnableVertexAttribArray ( pos );//默认是const 属性，数组型的属性需要额外开启！
    //make drawcall
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

}

NS_HEHE2D_END