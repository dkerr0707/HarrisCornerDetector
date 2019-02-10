//
//  GpuConvolution.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#pragma once

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "Convolution.hpp"

// The OpenCL set up code was modified from the following source:
// https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/

class GpuConvolution : public Convolution {
    
public:
    
    GpuConvolution() = delete;
    
    GpuConvolution(cv::Mat src);
    ~GpuConvolution();
    
    void Convolve(const cv::Mat& kernel, cv::Mat& result) override;
    
private:
    
    std::unique_ptr<char> m_deviceName;
    
    cl_context m_context;
    cl_command_queue m_command_queue;
    cl_kernel m_clKernel;
    cl_mem m_sourceMemoryObject;
    cl_mem m_resultMemoryObject;
    
    float *m_sourceImage;
    
};
