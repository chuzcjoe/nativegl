//
// Created by Zongcheng Chu on 6/29/24.
//

#include "ImageBase.h"

ImageBase::ImageBase(const char *file) : _file(file) {}

ImageBase::~ImageBase() {}

unsigned char *ImageBase::data() {
    return _data;
}

int ImageBase::getWidth() {
    return _width;
}

int ImageBase::getHeight() {
    return _height;
}