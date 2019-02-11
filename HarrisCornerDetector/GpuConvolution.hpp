//
//  GpuConvolution.hpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// The OpenCL set up code was modified from the following source:
// https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/

// GPU convolution using OpenGL. The code gets
// the first GPU device availble on the sytem
// and prints the device name to standard output.
//
// The source image and result buffer are loaded to
// the GPU at intialization. When the convolve function
// is called we only need to upload the convolution kernel
// and the dimension object to the GPU. This makes it
// efficient to do multiple convolutions on the same
// source image.

#pragma once

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "Convolution.hpp"

class GpuConvolution : public Convolution {
    
public:
    
    GpuConvolution() = delete;
    
    GpuConvolution(cv::Mat src);
    ~GpuConvolution();
    
    void Convolve(const cv::Mat& kernel, cv::Mat& result) override;
    
private:
    
    std::unique_ptr<char> m_deviceName;
    
    cl_context m_context;
    cl_command_queue m_commandQueue;
    cl_kernel m_clKernel;
    cl_mem m_sourceMemoryObject;
    cl_mem m_resultMemoryObject;
    
    float *m_sourceImage;
    
    size_t SetLocalItemSize(size_t globalItemSize);
    void ThrowOnError(cl_int ret) { if (ret != 0) throw std::runtime_error("OpenCL error : " + std::to_string(ret));};
    
};
