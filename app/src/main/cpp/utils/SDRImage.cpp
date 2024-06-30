//
// Created by Zongcheng Chu on 6/29/24.
//

#include "SDRImage.h"

SDRImage::SDRImage() : ImageBase() {}

SDRImage::SDRImage(const char *filename) : ImageBase(filename) {}

void SDRImage::loadJPEG() {
    const char* filename = _file;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Error: Cannot read file " << filename << std::endl;
        return;
    }

    tjhandle tjInstance = tjInitDecompress();
    if (tjInstance == nullptr) {
        std::cerr << "Error: tjInitDecompress failed" << std::endl;
        return;
    }

    int jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(tjInstance, buffer.data(), buffer.size(), &_width, &_height, &jpegSubsamp, &jpegColorspace) != 0) {
        std::cerr << "Error: tjDecompressHeader3 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return;
    }

    std::vector<unsigned char> imageBuffer(_width * _height * tjPixelSize[TJPF_RGB]);
    if (tjDecompress2(tjInstance, buffer.data(), buffer.size(), imageBuffer.data(), _width, 0, _height, TJPF_RGB, 0) != 0) {
        std::cerr << "Error: tjDecompress2 failed: " << tjGetErrorStr() << std::endl;
        tjDestroy(tjInstance);
        return;
    }

    // Flip the image vertically
    _data = new unsigned char[_width * _height * tjPixelSize[TJPF_RGB]];
    int rowSize = _width * tjPixelSize[TJPF_RGB];

    for (int y = 0; y < _height; ++y) {
        std::copy(
                imageBuffer.begin() + (_height - 1 - y) * rowSize,
                imageBuffer.begin() + (_height - y) * rowSize,
                _data + y * rowSize
        );
    }

    tjDestroy(tjInstance);
}