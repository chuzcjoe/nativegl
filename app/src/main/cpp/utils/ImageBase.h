//
// Created by Zongcheng Chu on 6/29/24.
//

#pragma once

#include "log.h"

class ImageBase {
public:
    ImageBase() = default;
    ImageBase(const char* file);

    virtual ~ImageBase();

    // Derived classes should decode jpeg in different ways
    virtual void loadJPEG() = 0;

    unsigned char* data();
    int getWidth();
    int getHeight();

protected:
    int _width;
    int _height;
    const char* _file;
    unsigned char* _data;
};
