//
// Created by Zongcheng Chu on 4/16/24.
//

#include "GLRender.h"
#include "GLShaderSource.h"

GLRender::GLRender(){

}
GLRender::~GLRender(){

}

void GLRender::cal_pixel() {
    vertices[0] = -0.5f; vertices[1] = -0.5f; vertices[2] = 0.0f;  // Left
    vertices[3] = 0.5f; vertices[4] = -0.5f; vertices[5] = 0.0f;  // Right
    vertices[6] = 0.0f; vertices[7] = 0.5f; vertices[8] = 0.0f;  // Top
}

void GLRender::SurfaceCreate() {
    tProgram = createProgram(vertexShader, fragmentShader);

    aPositionLocation  = glGetAttribLocation(tProgram, "a_Position");
    glClearColor(0.0,0.0,0.0,1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPositionLocation);
}

void GLRender::SurfaceChange(int width, int height) {
    glViewport(0, 0, width, height);
}


void GLRender::DrawFrame(){
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(tProgram); //使用texture的program
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);    //显示纹理
    glBindBuffer(GL_ARRAY_BUFFER, 0);   //用完buffer以后解绑定
}