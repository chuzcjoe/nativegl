//
// Created by Zongcheng Chu on 6/29/24.
//

#pragma once


class ImageBase {
public:
    ImageBase();
    ImageBase(const char* file);

    // Derived classes should decode jpeg in different ways
    virtual void loadJPEG() = 0;

    unsigned char* data();
    int getWidth();
    int getHeight();
    void setFilepath(const char* file);

protected:
    int _width;
    int _height;
    const char* _file;
    unsigned char* _data;
};
