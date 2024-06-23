//
// Created by Zongcheng Chu on 4/16/24.
//

#pragma once
#include "GLBase.h"
#include "turbojpeg.h"
#include "log.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class GLRender : public GLBase {
private:
    unsigned int tProgram;
    unsigned int aPositionLocation ;
    unsigned int aTexturePosition;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int _texture;

    float vertices[20];
    unsigned int indices[6];

    unsigned char* loadJPEG(const char* path, int& width, int& height);

public:
    GLRender();
    ~GLRender();
    void cal_pixel();

    virtual void surfaceCreate() override;
    virtual void surfaceChange(int width, int height) override;
    virtual void drawFrame() override;
};

