//
// Created by Zongcheng Chu on 4/16/24.
//

#include "GLRender.h"
#include "GLShaderSource.h"

GLRender::GLRender() {}
GLRender::~GLRender() {}

void GLRender::cal_pixel() {
    vertices[0] = -1.0f; vertices[1] = -1.0f; vertices[2] = 0.0f; vertices[3] = 0.0f; vertices[4] = 0.0f;  // Bottom left
    vertices[5] = 1.0f; vertices[6] = -1.0f; vertices[7] =  0.0f; vertices[8] = 1.0f; vertices[9] = 0.0f; // Bottom right
    vertices[10] = 1.0f;  vertices[11] = 1.0f; vertices[12] = 0.0f; vertices[13] = 1.0f; vertices[14] = 1.0f; // Top right
    vertices[15] = -1.0f;  vertices[16] = 1.0f; vertices[17] = 0.0f; vertices[18] = 0.0f; vertices[19] = 1.0f;  // Top left

    // indices
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
}

unsigned char *GLRender::loadJPEG(const char *filename, int &width, int &height) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return nullptr;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Error: Cannot read file " << filename << std::endl;
        return nullptr;
    }

    tjhandle tjInstance = tjInitDecompress();
    if (tjInstance == nullptr) {
        std::cerr << "Error: tjInitDecompress failed" << std::endl;
        return nullptr;
    }

    int jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(tjInstance, buffer.data(), buffer.size(), &width, &height, &jpegSubsamp, &jpegColorspace) != 0) {
        std::cerr << "Error: tjDecompressHeader3 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return nullptr;
    }

    std::vector<unsigned char> imageBuffer(width * height * tjPixelSize[TJPF_RGB]);
    if (tjDecompress2(tjInstance, buffer.data(), buffer.size(), imageBuffer.data(), width, 0, height, TJPF_RGB, 0) != 0) {
        std::cerr << "Error: tjDecompress2 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return nullptr;
    }

    // Flip the image vertically
    unsigned char *flippedImage = new unsigned char[width * height * tjPixelSize[TJPF_RGB]];
    int rowSize = width * tjPixelSize[TJPF_RGB];

    for (int y = 0; y < height; ++y) {
        std::copy(
                imageBuffer.begin() + (height - 1 - y) * rowSize,
                imageBuffer.begin() + (height - y) * rowSize,
                flippedImage + y * rowSize
        );
    }

    tjDestroy(tjInstance);
    return flippedImage;
}

void GLRender::surfaceCreate() {
    tProgram = createProgram(vertexShader, fragmentShader);
    aPositionLocation  = glGetAttribLocation(tProgram, "a_Position");
    aTexturePosition = glGetAttribLocation(tProgram, "a_Texture");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    int width, height;
    glGenTextures(1, &_texture);
    glActiveTexture(GL_TEXTURE0); // activated by default
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto data = loadJPEG("/data/local/tmp/jpeg_demo/hdr.jpg", width, height);

    if (data) {
        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        LOG_INFO("load texture success, width: %d, height: %d\n", width, height);

    } else {
        LOG_ERROR("load texture error");
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPositionLocation);

    glVertexAttribPointer(aTexturePosition, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(aTexturePosition);

    glUseProgram(tProgram);
    glUniform1i(glGetUniformLocation(tProgram, "texture_load"), 0);
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