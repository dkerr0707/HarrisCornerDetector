//
//  Harris.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include "CornerDetector.hpp"
#include "Filters.hpp"

#include <opencv2/core/core.hpp>

class Harris : public CornerDetector {

public:
    Harris() = delete;
    Harris(cv::Mat& src, bool gpu) :
        CornerDetector(src),
        m_filter(GetGray(), gpu) {};
    
    ~Harris() {};
    
    std::vector<cv::Point> GetCorners() override;
    
private:
    
    Filters m_filter;
    
    double Normalize(unsigned char c) {
        return c / 255.0;
    }
    
};
