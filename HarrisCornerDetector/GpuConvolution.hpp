//
//  GpuConvolution.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include "Convolution.hpp"

class GpuConvolution : public Convolution {
    
public:
    
    GpuConvolution() = delete;
    GpuConvolution(cv::Mat src) : Convolution(src) {};
    
    void Convolve(const cv::Mat& kernel, cv::Mat& result) override;

};
