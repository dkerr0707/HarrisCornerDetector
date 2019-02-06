//
//  Harris.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Harris.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

void Harris::Run() {
    
    // 1. Pre-filter the image 𝐼 with a Gaussian kernel 𝐺𝜎 with some sigma
    cv::Size kernelSize(5, 5);
    cv::Mat blurred;
    
    cv::GaussianBlur( GetGray(), blurred, kernelSize, 0, 0 );
    
    // 2. Compute the horizontal and vertical image gradients, 𝐼𝑥 and 𝐼𝑦, respectively.
    int scale = 1;
    int delta = 0;
    int depth = CV_16S;
    cv::Mat gradientX;
    cv::Mat gradientXAbs;
    
    cv::Sobel( GetGray(), gradientX, depth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( gradientX, gradientXAbs );
    
    cv::Mat gradientY;
    cv::Mat gradientYAbs;
    cv::Sobel( GetGray(), gradientY, depth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( gradientY, gradientYAbs );
    
    // 3. Compute the sum of product derivatives in a NxN window, where ‘N’ is an odd value
    cv::Size s = gradientYAbs.size();
    std::cout << s.width << " " << s.height << std::endl;
    
    int N = 5;
    assert(N % 2 != 0); // N must be odd
    
    cv::Size blocks(s.width / N, s.height / N);
    std::cout << blocks.width << " " << blocks.height << std::endl;
    
    std::cout << gradientXAbs.type() << std::endl;
    
    
    
    for (int r = 0; r < gradientXAbs.size().height; r++) {
        for (int c = 0; c < gradientXAbs.size().width; c++) {
            std::cout << int(gradientXAbs.at<uchar>(r,c)) << " ";
        }
        std::cout << std::endl;
    }
    
    
    const std::string windowName2 = "Gradient X";
    namedWindow(windowName2, cv::WINDOW_AUTOSIZE );
    imshow(windowName2, gradientXAbs );
    
}
