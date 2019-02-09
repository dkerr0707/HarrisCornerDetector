//
//  Convolution.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Convolution.hpp"
#include "CpuConvolution.hpp"

std::unique_ptr<Convolution> Convolution::Create(cv::Mat src, bool gpu) {
    return std::unique_ptr<CpuConvolution>(new CpuConvolution(src));
}
