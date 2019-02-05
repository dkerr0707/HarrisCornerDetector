//
//  OpenCV_Harris.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "OpenCV_Harris.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

void OpenCV_Harris::Run() {
    
    cornerHarris_demo( 0, 0 );
    
}

// This function is copied from here :
// https://docs.opencv.org/3.4/d4/d7d/tutorial_harris_detector.html
void OpenCV_Harris::cornerHarris_demo( int, void* )
{
    
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    
    cv::Mat dst = cv::Mat::zeros( GetSource().size(), CV_32FC1 );
    cornerHarris( GetGray(), dst, blockSize, apertureSize, k );
    
    cv::Mat dst_norm, dst_norm_scaled;
    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > m_thresh )
            {
                circle( dst_norm_scaled, cv::Point(j,i), 5,  cv::Scalar(0), 2, 8, 0 );
            }
        }
    }
    const char* corners_window = "Corners detected";
    cv::namedWindow( corners_window );
    imshow( corners_window, dst_norm_scaled );
}

