//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once
#include "GLBase.h"

class GLRender : public GLBase {
private:
    unsigned int tProgram;
    unsigned int aPositionLocation ;
    unsigned int VAO;
    unsigned int VBO;

    float vertices[9];

public:
    GLRender();
    ~GLRender();
    void cal_pixel();

    //实现OpenGL渲染上下文的回调函数
    virtual void surfaceCreate();
    virtual void surfaceChange(int width, int height);
    virtual void drawFrame();
};

