//
//  Filters.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Filters.hpp"

#include <iostream>

void Filters::Soble(cv::Mat& result, Type type) {
    
    cv::Mat kernel;
    
    switch (type) {
        case SOBEL_X:
            kernel = (cv::Mat_<float>(3, 3) <<
                      -1, 0, 1,
                      -2, 0, 2,
                      -1, 0, 1);
            break;
            
        case SOBEL_Y:
            kernel = (cv::Mat_<float>(3, 3) <<
                      -1, -2, -1,
                      0, 0, 0,
                      1, 2, 1);
            break;
            
        default:
            throw std::runtime_error("Soble Type error.");
    };
    
    cv::Mat convolutionResult;
    m_convolution->Convolve(kernel, convolutionResult);
    
    // Return the scaled abs result
    // unsigned 8 bit 0 -> 255 range
    // This if for consistency and easy debugging
    convertScaleAbs( convolutionResult, result );
}

void Filters::Gaussian(cv::Mat& result, unsigned int kernelDim, double sigma) {
    
    // kernel dimensions must odd
    assert(kernelDim % 2 != 0);
    
    cv::Mat kernel(kernelDim, kernelDim, CV_32FC1, cv::Scalar(0));
    int indexOffset = kernelDim / 2;
    
    for (int y = -indexOffset; y <= indexOffset; y++ ) {
        for (int x = -indexOffset; x <= indexOffset; x++) {
            double gaussian = ((1 / (2 * M_PI * sigma * sigma)) * exp( - (pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2))));
            kernel.at<float>(y + indexOffset, x + indexOffset) = gaussian;
        }
    }
    
    cv::Mat convolutionResult;
    m_convolution->Convolve(kernel, convolutionResult);
    
    // Return the scaled abs result
    // unsigned 8 bit 0 -> 255 range
    // This if for consistency and easy debugging
    convertScaleAbs( convolutionResult, result );

}
