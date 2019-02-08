//
//  Harris.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Harris.hpp"
#include "Filters.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <iostream>

void Harris::Run() {
    
    // 1. Pre-filter the image 𝐼 with a Gaussian kernel 𝐺𝜎 with some sigma
    cv::Mat blurred;
    Filters::Gaussian(GetGray(), blurred);
    
    // 2. Compute the horizontal and vertical image gradients, 𝐼𝑥 and 𝐼𝑦, respectively.
    cv::Mat gradientX;
    Filters::Soble(GetGray(), gradientX, Filters::Type::SOBEL_X);
    
    cv::Mat gradientY;
    Filters::Soble(GetGray(), gradientY, Filters::Type::SOBEL_Y);
    
    // 3. Compute the sum of product derivatives in a NxN window, where ‘N’ is an odd value
    int N = 5;
    assert(N % 2 != 0); // N must be odd
    
    cv::Size s = gradientX.size();
    cv::Size blocks(s.width / N, s.height / N);
    
    for (int blockRow = 0; blockRow < blocks.height; blockRow++) {
        for (int blockColumn = 0; blockColumn < blocks.width; blockColumn++) {
            
            // 4. Compute the structure tensor
            double Sxx(0);
            double Sxy(0);
            double Syy(0);
            
            int rowStart = N * blockRow;
            int columnStart = N * blockColumn;
            
            for (int r = rowStart; r < rowStart + N; r++) {
                for (int c = columnStart; c < columnStart + N; c++) {
                    
                    Sxx += pow(gradientX.at<uchar>(r, c), 2);
                    Sxy += gradientX.at<uchar>(r, c) * gradientY.at<uchar>(r, c);
                    Syy += pow(gradientY.at<uchar>(r, c), 2);
                    
                }
            }
            
            // 5. Compute the response
            double k = 0.04;
            double R = ((Sxx * Syy) - (Sxy * Sxy)) - k * pow((Sxx + Syy), 2);

            // 6. Apply non-maximum suppression in a NxN window with thresholding to extract the final corner locations
            // 𝑇ℎ = 𝑇 ∗ max𝑅(𝑥, 𝑦)
            if (R > 0) {
                cv::Point p = cv::Point( columnStart + N / 2, rowStart + N / 2 );
                circle( GetSource(), p, 5, cv::Scalar( 0, 0, 255 ), 2, 8 );
                
                std::cout << p.x << " " << p.y << std::endl;
            }
            
        }
    }
    
    const std::string windowName2 = "Corners";
    namedWindow(windowName2, cv::WINDOW_AUTOSIZE );
    imshow(windowName2, GetSource() );
    
}
