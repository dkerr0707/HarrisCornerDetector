//
//  CpuConvolution.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// CPU convolution implementation.

#pragma once

#include "Convolution.hpp"

class CpuConvolution : public Convolution {

public:
    
    CpuConvolution() = delete;
    CpuConvolution(cv::Mat src) : Convolution(src) {};
    
    void Convolve(const cv::Mat& kernel, cv::Mat& result) override;
};
