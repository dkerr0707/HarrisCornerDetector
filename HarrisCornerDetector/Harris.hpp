//
//  Harris.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include "CornerDetector.hpp"

#include <opencv2/core/core.hpp>

class Harris : public CornerDetector {

public:
    Harris() = delete;
    Harris(cv::Mat& src) : CornerDetector(src) {};
    ~Harris() {};
    
    void Run() override;
    
};
