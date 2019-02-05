//
//  CornerDetector.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include <opencv2/core/core.hpp>

class CornerDetector {

public:
    CornerDetector(cv::Mat src);
    virtual void Run() = 0;
    
protected:
    cv::Mat GetSource() const { return m_src; };
    cv::Mat GetGray() const { return m_srcGray; };

private:
    cv::Mat m_src;
    cv::Mat m_srcGray;
};
