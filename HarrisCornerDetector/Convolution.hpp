//
//  Convolution.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-08.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

namespace Convolution {
    
    static cv::Mat Convolve(const cv::Mat& kernel, const cv::Mat& img) {
        
        // We only accept odd row and odd col kernels
        assert(kernel.rows % 2 != 0 &&
               kernel.cols % 2 != 0);
        
        // convert to floating point percision
        cv::Mat convertedKernel;
        kernel.convertTo(convertedKernel, CV_32F);
        
        cv::Mat convertedImg;
        img.convertTo(convertedImg, CV_32F);
        
        cv::Mat result = cv::Mat::zeros(cv::Size(convertedImg.cols, convertedImg.rows), CV_32F);;
        
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
        
        
        
        cv::Mat abs;
        convertScaleAbs( result, abs );
        const std::string windowName2 = "bling";
        namedWindow(windowName2, cv::WINDOW_AUTOSIZE );
        imshow(windowName2, abs );
        
    //    std::cout << abs << std::endl;
        
        
        return result;
    }
    
};
