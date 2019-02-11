//
//  Convolution.h
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// Convolution interface implemented using a factory
// design pattern. Call the create function to get
// the appropriate convoltion hardware setup. Options are
// GPU and CPU hardware setups.
//
// The input image is restricted to monochromatic
// images.
//
// Convolution kernels must have odd dimensions.
//
// The convolution result has the same dimensions at the
// input image. The convolution algorithm pads the image
// borders with zeros. The padding size will be the
// floor(kernelDimension / 2).

#pragma once

#include <opencv2/core/core.hpp>

class Convolution {
public:
    
    // Factory to switch between GPU and CPU implementations
    static std::unique_ptr<Convolution> Create(cv::Mat src, bool gpu);
    
    virtual void Convolve(const cv::Mat& kernel, cv::Mat& result) = 0;
    
    cv::Mat& GetSource() { return m_src; }

protected:
    
    // only base classes can construct this object
    Convolution(cv::Mat src);
    
private:
    
    cv::Mat m_src;
};
