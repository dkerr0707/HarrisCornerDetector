//
//  Convolution.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Convolution.hpp"
#include "CpuConvolution.hpp"
#include "GpuConvolution.hpp"

Convolution::Convolution(cv::Mat src) {

    // We only handle single channel images
    assert(src.channels() == 1);
    
    src.convertTo(m_src, CV_32F);
    
}

std::unique_ptr<Convolution> Convolution::Create(cv::Mat src, bool gpu) {
    
    if (gpu) {
        return std::unique_ptr<Convolution>(new GpuConvolution(src));
    }
    
    return std::unique_ptr<Convolution>(new CpuConvolution(src));
}
