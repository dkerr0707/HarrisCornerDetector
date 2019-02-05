//
//  Application.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Application.hpp"
#include "OpenCV_Harris.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

Application::Application(int argc, char** argv) {
    
    std::string path = ValidateArgs(argc, argv);
    
    m_src = LoadImage(path);
    
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    imshow( "Display window", m_src );
    
}

void Application::Run() {
    
    std::unique_ptr<CornerDetector> detector(new OpenCV_Harris(m_src));
    detector->Run();
    
    cv::waitKey(0);
    
}

std::string Application::ValidateArgs(int argc, char** argv) {
    
    if( argc != 2)
    {
        throw std::runtime_error("Invalid Arguments - HarrisCornerDetector IMAGE_PATH");
    }
    
    return argv[1];
    
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
