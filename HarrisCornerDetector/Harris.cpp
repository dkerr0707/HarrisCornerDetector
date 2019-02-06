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

void Harris::Run() {
    
    const std::string windowName = "Gray";
    namedWindow(windowName, cv::WINDOW_AUTOSIZE );
    imshow(windowName, GetGray() );
    
    cv::Mat blurred;
    cv::Size kernelSize(5, 5);
    
    cv::GaussianBlur( GetGray(), blurred, kernelSize, 0, 0 );
    
    const std::string windowName2 = "Blurred";
    namedWindow(windowName2, cv::WINDOW_AUTOSIZE );
    imshow(windowName2, blurred );
    
}
