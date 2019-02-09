//
//  Filters.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-08.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include "Convolution.hpp"

#include <opencv2/core/core.hpp>

class Filters {

public:
    
    Filters() = delete;
    Filters(cv::Mat src, bool gpu) : convolution(Convolution::Create(src, gpu)) {};
    
    enum Type { SOBEL_X, SOBEL_Y };
    
    void Soble(cv::Mat& result, Type type);
    void Gaussian(cv::Mat& result, unsigned int kernelDim, double sigma);
    
private:
    
    std::unique_ptr<Convolution> convolution;
    
};
