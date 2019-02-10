//
//  CornerDetector.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "CornerDetector.hpp"

#include <opencv2/imgproc.hpp>

CornerDetector::CornerDetector(const cv::Mat& src) :
    m_src(src) {
    
    cv::cvtColor( m_src, m_srcGray, cv::COLOR_BGR2GRAY );
    
}
