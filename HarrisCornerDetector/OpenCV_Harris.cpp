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

std::vector<cv::Point> OpenCV_Harris::GetCorners() {
    
    cornerHarris_demo( 0, 0 );
    
    return m_corners;
    
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
                cv::Point p = cv::Point(j,i);
                circle( dst_norm_scaled, p, 5,  cv::Scalar(0), 2, 8, 0 );
                m_corners.push_back(p);
            }
        }
    }
    const char* corners_window = "Corners detected";
    cv::namedWindow( corners_window );
    imshow( corners_window, dst_norm_scaled );
}

