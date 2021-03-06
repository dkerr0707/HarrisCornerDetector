//
//  Filters.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-08.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// Filters class holds the filter related data and
// algorithms. Soble filter uses the standard 3x3
// filters. Gaussian creates a filter based on the
// kernel dimensions and sigma. All convoltion kernels
// are square.

#pragma once

#include "Convolution.hpp"

#include <opencv2/core/core.hpp>

class Filters {

public:
    
    Filters() = delete;
    Filters(cv::Mat src, bool gpu) : m_convolution(Convolution::Create(src, gpu)) {};
    
    enum Type { SOBEL_X, SOBEL_Y };
    
    void Soble(cv::Mat& result, Type type);
    void Gaussian(cv::Mat& result, unsigned int kernelDim, double sigma);
    
private:
    
    std::unique_ptr<Convolution> m_convolution;
    
};
