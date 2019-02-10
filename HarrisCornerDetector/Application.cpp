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
#include <fstream>

Application::Application(int argc, char** argv) :
    m_path(""),
    m_gpu(false),
    m_reportPath(""){
    
    ValidateArgs(argc, argv);
    
    m_src = LoadImage(m_path);
    
    // Use this detector to test against OpenCVs implementation
    //m_detector = std::unique_ptr<CornerDetector>(new OpenCV_Harris(m_src));
        
    m_detector = std::unique_ptr<CornerDetector>(new Harris(m_src, m_gpu));
    
}

void Application::Run() {
    
    auto t1 = std::chrono::high_resolution_clock::now();
    
    std::vector<cv::Point> corners = m_detector->GetCorners();
    
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    
    CreateReport(corners, time);
    
    
}

// We can write to a file or std out wit this function.
void Application::CreateReport(const std::vector<cv::Point>& corners,
                               const std::chrono::duration<long long, std::__1::ratio<1, 1000> >& time) {
    
    bool writeToFile = m_reportPath.compare("") != 0;
    
    std::streambuf *coutbuf = NULL;
    std::unique_ptr<std::ofstream> out;
    if (writeToFile) {
        out = std::unique_ptr<std::ofstream>(new std::ofstream(m_reportPath));
        coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out->rdbuf());
    }
    
    std::cout << "Harris Corner Detector Report" << std::endl;
    std::cout << std::endl;
    
    std::cout << "GPU used : ";
    if (m_gpu) {
        std::cout << "True" << std::endl;
    }
    else {
        std::cout << "False" << std::endl;
    }
    
    std::cout << "Duration : " << time.count() << " ms" << std::endl;
    std::cout << std::endl;
    
    std::cout << corners.size() << " corners found." << std::endl;
    std::cout << std::endl;
    
    std::cout << "Approximate X Y Positions:" << std::endl;
    
    for(auto p: corners) {
        std::cout << p.x << " " << p.y << std::endl;
        circle( m_src, p, 5, cv::Scalar( 0, 0, 255 ), 2, 8 );
    }
    
    if (writeToFile) {
        std::cout.rdbuf(coutbuf);
    }
    
    const std::string windowName = "Corners";
    namedWindow(windowName, cv::WINDOW_AUTOSIZE );
    imshow(windowName, m_src);
    
    cv::waitKey(0);
    
}

void Application::ValidateArgs(int argc, char** argv) {
    
    if( argc < 2)
    {
        throw std::runtime_error("Usage - HarrisCornerDetector IMAGE_PATH CPU/GPU REPORT_PATH");
    }
    
    m_path = argv[1];
    
    if (argc > 2 && std::string(argv[2]).compare("GPU") == 0) {
        m_gpu = true;
    }
    
    if (argc > 3 ) {
        m_reportPath = argv[3];
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




