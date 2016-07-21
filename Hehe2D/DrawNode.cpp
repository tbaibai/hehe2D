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
    Size winSize = Renderer::instance()->getWinSize();
    GLfloat w = 100, h = 100;

    Renderer::instance()->addShaderProgram("matrixTest", "matTest.vs", "matTest.fs");
    GLuint obj = Renderer::instance()->useShaderProgram("matrixTest");

    Matrix tran;
    EsHelper::matrixLoadIdentity(tran);
    EsHelper::scale(tran, 2.f / winSize.w, 2.f / winSize.h, 1.f);//最先左乘投影
    EsHelper::translate(tran, pos_.x, pos_.y, 0.f);
    EsHelper::rotate(tran, 5);

    GLuint matLoc = glGetUniformLocation(obj, "u_m");
    glUniformMatrix4fv(matLoc, 1,false, &(tran.m[0][0]));

    GLfloat vertData[] = {
        0.5*w, 0.5*h, 0.f,
        -0.5*w, 0.5*h, 0.f,
        -0.5*w, -0.5*h, 0.f,
        0.5*w, -0.5*h, 0.f,
    };
    GLuint pos = glGetAttribLocation ( Renderer::instance()->getShaderProgram("matrixTest"), "a_position" );
    glVertexAttribPointer ( pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertData );
    glEnableVertexAttribArray ( pos );//默认是const 属性，数组型的属性需要额外开启！
    //make drawcall
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

}

NS_HEHE2D_END