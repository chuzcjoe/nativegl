//
// Created by Zongcheng Chu on 4/16/24.
//

#include "GLRender.h"
#include "GLShaderSource.h"

GLRender::GLRender() {}
GLRender::~GLRender() {}

void GLRender::cal_pixel() {
    vertices[0] = -0.5f; vertices[1] = -0.5f; vertices[2] = 0.0f;  // Bottom left
    vertices[3] = 0.5f; vertices[4] = -0.5; vertices[5] =  0.0f;  // Bottom right
    vertices[6] = 0.5f;  vertices[7] = 0.5f; vertices[8] = 0.0f;  // Top right
    vertices[9] = -0.5f;  vertices[10] = 0.5f; vertices[11] = 0.0f;   // Top left

    // indices
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
}

void GLRender::surfaceCreate() {
    tProgram = createProgram(vertexShader, fragmentShader);
    aPositionLocation  = glGetAttribLocation(tProgram, "a_Position");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPositionLocation);
}

void GLRender::surfaceChange(int width, int height) {
    glViewport(0, 0, width, height);
}


void GLRender::drawFrame() {
    glClearColor(1., 1., 1., 1.); // White background
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(tProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}