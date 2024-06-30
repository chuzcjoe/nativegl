//
// Created by Zongcheng Chu on 6/29/24.
//

#pragma once

#include "ImageBase.h"
#include "turbojpeg.h"
#include "log.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>



class SDRImage : public ImageBase{
public:
    SDRImage();
    SDRImage(const char* filename);

    void loadJPEG() override;
};
