//
//  Filters.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-08.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include <opencv2/core/core.hpp>

namespace Filters {

    static void Convolution(const cv::Mat& kernel, const cv::Mat& img, cv::Mat& result) {
        
        // We only accept odd row and odd col kernels
        assert(kernel.rows % 2 != 0 &&
               kernel.cols % 2 != 0);
        
        // We only handle single channel images
        assert(kernel.channels() == 1 &&
               img.channels() == 1);
        
        // convert to floating point percision
        cv::Mat convertedKernel;
        kernel.convertTo(convertedKernel, CV_32F);
        
        cv::Mat convertedImg;
        img.convertTo(convertedImg, CV_32F);
        
        cv::Mat convolvedImage = cv::Mat::zeros(cv::Size(convertedImg.cols, convertedImg.rows), CV_32F);;
        
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
                        
                        convolvedImage.at<float>(r, c) +=
                            convertedKernel.at<float>(kernelRowIndex, kernelColIndex) *
                            convertedImg.at<float>(imageRowIndex, imageColIndex);
                        
                    }
                }

            }
        }
        
        // Return the scaled abs result
        // unsigned 8 bit 0 -> 255 range
        // This if for consistency and easy debugging
        convertScaleAbs( convolvedImage, result );
        
    }
    
    enum Type { SOBEL_X, SOBEL_Y };
    
    static void Soble(const cv::Mat& img, cv::Mat& result, Type type) {
        
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
        
        
        Convolution(kernel, img, result);
    }
    
    static void Gaussian(const cv::Mat& img, cv::Mat& result, unsigned int kernelDim, double sigma) {
        
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
        
        
        Convolution(kernel, img, result);
    }

    
};
