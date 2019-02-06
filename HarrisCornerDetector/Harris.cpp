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
    
    cv::Size kernelSize(5, 5);
    cv::Mat blurred;
    
    cv::GaussianBlur( GetGray(), blurred, kernelSize, 0, 0 );
    
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
    
    cv::Size s = gradientYAbs.size();
    std::cout << s.width << " " << s.height << std::endl;
    
    
    
    
    
    const std::string windowName2 = "Gradient X";
    namedWindow(windowName2, cv::WINDOW_AUTOSIZE );
    imshow(windowName2, gradientXAbs );
    
    const std::string windowName3 = "Gradient Y";
    namedWindow(windowName3, cv::WINDOW_AUTOSIZE );
    imshow(windowName3, gradientYAbs );
}
