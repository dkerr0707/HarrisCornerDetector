//
//  Application.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Application.hpp"
#include "OpenCV_Harris.hpp"
#include "Harris.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <iostream>

Application::Application(int argc, char** argv) :
    m_path(""),
    m_gpu(false) {
    
    ValidateArgs(argc, argv);
    
    m_src = LoadImage(m_path);
    
    // Use this detector to test against OpenCVs implementation
    //m_detector = std::unique_ptr<CornerDetector>(new OpenCV_Harris(m_src));
        
    m_detector = std::unique_ptr<CornerDetector>(new Harris(m_src, m_gpu));
    
}

void Application::Run() {
    
    auto t1 = std::chrono::high_resolution_clock::now();
    
    m_detector->Run();
    
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << time.count() << " ms" << std::endl;
    
    cv::waitKey(0);
    
}

void Application::ValidateArgs(int argc, char** argv) {
    
    if( argc < 2)
    {
        throw std::runtime_error("Usage - HarrisCornerDetector IMAGE_PATH CPU/GPU");
    }
    
    m_path = argv[1];
    if (argc > 2 && std::string(argv[2]).compare("GPU") == 0) {
        m_gpu = true;
    }
    
}

cv::Mat Application::LoadImage(std::string path) {
    
    cv::Mat image;
    image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
    
    if(! image.data )
    {
        throw std::runtime_error("File not found : " + path);
    }
    
    return image;
    
}




