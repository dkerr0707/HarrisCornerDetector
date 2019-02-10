//
//  Convolution.h
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include <opencv2/core/core.hpp>

class Convolution {
public:
    
    // Factory to switch between GPU and CPU implementations
    static std::unique_ptr<Convolution> Create(cv::Mat src, bool gpu);
    
    virtual void Convolve(const cv::Mat& kernel, cv::Mat& result) = 0;
    
    cv::Mat& GetSource() { return m_src; }

protected:
    
    Convolution(cv::Mat src);
    
private:
    
    cv::Mat m_src;
};
