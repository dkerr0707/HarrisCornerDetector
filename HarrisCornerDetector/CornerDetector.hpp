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
    CornerDetector(const cv::Mat& src, bool gpu);
    virtual ~CornerDetector() {};
    
    virtual void Run() = 0;
    void EnableGPU() { m_gpu = true; }
    void DisableGPU() { m_gpu = false; }
    
protected:
    cv::Mat GetSource() const { return m_src; };
    cv::Mat GetGray() const { return m_srcGray; };
    
    bool GpuEnabled() const { return m_gpu; }

private:
    cv::Mat m_src;
    cv::Mat m_srcGray;
    
    bool m_gpu;
};
