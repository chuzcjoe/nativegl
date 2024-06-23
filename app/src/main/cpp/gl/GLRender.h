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
    unsigned int EBO;

    float vertices[12];
    unsigned int indices[6];

public:
    GLRender();
    ~GLRender();
    void cal_pixel();

    virtual void surfaceCreate() override;
    virtual void surfaceChange(int width, int height) override;
    virtual void drawFrame() override;
};

