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
    
    Filters filter(GetGray(), GpuEnabled());
    
    // 1. Pre-filter the image 𝐼 with a Gaussian kernel 𝐺𝜎 with some sigma
    cv::Mat blurred;
    unsigned int kernelDim = 5;
    double sigma = 1;
    filter.Gaussian(blurred, kernelDim, sigma);
    
    // 2. Compute the horizontal and vertical image gradients, 𝐼𝑥 and 𝐼𝑦, respectively.
    cv::Mat gradientX;
    filter.Soble(gradientX, Filters::Type::SOBEL_X);
    
    cv::Mat gradientY;
    filter.Soble(gradientY, Filters::Type::SOBEL_Y);
    
    // 3. Compute the sum of product derivatives in a NxN window, where ‘N’ is an odd value
    int N = 5;
    assert(N % 2 != 0); // N must be odd
    
    cv::Size s = gradientX.size();
    cv::Size blocks(s.width / N, s.height / N);
    
    // This was tricky to figure out. Were looking for the biggest change.
    // So I feel that the max change would be half of each gradient block being
    // high (255) and the other half being low (0). Because these value are being
    // muliplied I squared the final value.
    // N^2 is the block size. Divide that by two to get half blocks being high.
    // Then square the result to get the max.
    double maxR = pow(pow(N, 2) / 2, 2);
    
    // 𝑇ℎ = 𝑇 ∗ max𝑅(𝑥, 𝑦)
    double T = 0.3;
    double threshold = T * maxR;
    
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
                    
                    Sxx += pow(Normalize(gradientX.at<uchar>(r, c)), 2);
                    Sxy += Normalize(gradientX.at<uchar>(r, c)) * Normalize(gradientY.at<uchar>(r, c));
                    Syy += pow(Normalize(gradientY.at<uchar>(r, c)), 2);
                    
                }
            }
            
            // 5. Compute the response
            double k = 0.04;
            double R = ((Sxx * Syy) - (Sxy * Sxy)) - k * pow((Sxx + Syy), 2);

            // 6. Apply non-maximum suppression in a NxN window with thresholding to extract the final corner locations
            if (R > threshold) {
                
                int blockCenter = N / 2;
                cv::Point p = cv::Point( columnStart + blockCenter, rowStart + blockCenter);
                
                circle( GetSource(), p, 5, cv::Scalar( 0, 0, 255 ), 2, 8 );
                std::cout << p.x << " " << p.y << std::endl;
            }
            
        }
    }
    
    const std::string windowName = "Corners";
    namedWindow(windowName, cv::WINDOW_AUTOSIZE );
    imshow(windowName, GetSource());
    
}
