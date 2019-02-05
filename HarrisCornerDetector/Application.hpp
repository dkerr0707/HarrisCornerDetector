//
//  Application.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#include <opencv2/core/core.hpp>

class Application {
public:
    Application() = delete;
    Application(int argc, char** argv);
    void Run();
    
private:
    std::string ValidateArgs(int argc, char** argv);
    cv::Mat LoadImage(std::string path);
    
    cv::Mat m_src;
    
};
