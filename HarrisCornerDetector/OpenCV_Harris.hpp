//
//  OpenCV_Harris.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// This class is based on the example code that can be found here :
// https://docs.opencv.org/3.4/d4/d7d/tutorial_harris_detector.html

#pragma once

#include "CornerDetector.hpp"

#include <opencv2/core/core.hpp>

class OpenCV_Harris : public CornerDetector {
    
public:
    OpenCV_Harris() = delete;
    OpenCV_Harris(cv::Mat src) : CornerDetector(src) {};
    ~OpenCV_Harris() {};
    
    void Run() override;
    
private:
    cv::Mat m_src;
    cv::Mat m_src_gray;
    const int m_thresh = 200;
    
    void cornerHarris_demo( int, void* );
    
};
