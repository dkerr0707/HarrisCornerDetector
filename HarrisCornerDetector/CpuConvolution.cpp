//
//  CpuConvolution.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "CpuConvolution.hpp"

void CpuConvolution::Convolve(const cv::Mat& kernel, cv::Mat& result) {
    
    // We only accept odd row and odd col kernels
    assert(kernel.rows % 2 != 0 &&
           kernel.cols % 2 != 0);
    
    // We only handle single channel kernels
    assert(kernel.channels() == 1);
    
    // convert to floating point percision
    cv::Mat convertedKernel;
    kernel.convertTo(convertedKernel, CV_32F);
    
    cv::Mat convertedImg = GetSource();
    
    result = cv::Mat::zeros(cv::Size(convertedImg.cols, convertedImg.rows), CV_32F);;
    
    // We add a border to the output image with this technique
    // the border will the floor(kernel.dimension / 2)
    int rowOffset = kernel.rows / 2;
    for (int r = rowOffset; r < convertedImg.rows - rowOffset; r++) {
        
        int colOffset = kernel.cols / 2;
        for (int c = colOffset; c < convertedImg.cols - colOffset; c++) {
            
            for (int currentRow = -rowOffset; currentRow <= rowOffset; currentRow++) {
                for (int currentCol = -colOffset; currentCol <= colOffset; currentCol++) {
                    
                    // kernel space index
                    int kernelRowIndex = currentRow + rowOffset;
                    int kernelColIndex = currentCol + colOffset;
                    
                    // image space index
                    int imageRowIndex = r + currentRow;
                    int imageColIndex = c + currentCol;
                    
                    result.at<float>(r, c) +=
                        convertedKernel.at<float>(kernelRowIndex, kernelColIndex) *
                        convertedImg.at<float>(imageRowIndex, imageColIndex);
                    
                }
            }
            
        }
    }
    
}
