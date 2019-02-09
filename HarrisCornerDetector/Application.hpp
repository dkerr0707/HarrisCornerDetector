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
    void ValidateArgs(int argc, char** argv);
    cv::Mat LoadImage(std::string path);
    
    std::string m_path;
    bool m_gpu;
    cv::Mat m_src;
    
};
