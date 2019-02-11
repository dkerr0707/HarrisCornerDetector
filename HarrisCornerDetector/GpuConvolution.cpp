//
//  GpuConvolution.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "GpuConvolution.hpp"
#include "OpenCL_Filter.hpp"

#include <iostream>

GpuConvolution::GpuConvolution(cv::Mat src) : Convolution(src) {
    
    // The OpenCL set up code was modified from the following source:
    // https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
    
    cl_platform_id platformId = NULL;
    cl_device_id deviceId = NULL;
    cl_uint numDevices;
    cl_uint numPlatforms;
    cl_int ret = clGetPlatformIDs(1, &platformId, &numPlatforms);
    ThrowOnError(ret);
    
    ret = clGetDeviceIDs( platformId, CL_DEVICE_TYPE_DEFAULT, 1,
                             &deviceId, &numDevices);
    ThrowOnError(ret);
    
    // print device name
    size_t nameSize;
    clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &nameSize);
    m_deviceName = std::unique_ptr<char>(new char[nameSize]);
    clGetDeviceInfo(deviceId, CL_DEVICE_NAME, nameSize, m_deviceName.get(), NULL);
    std::cout << m_deviceName.get() << std::endl;
    
    m_context = clCreateContext( NULL, 1, &deviceId, NULL, NULL, &ret);
    m_commandQueue = clCreateCommandQueue(m_context, deviceId, 0, &ret);
    
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(m_context, 1,
                                                   (const char **)&s_openClKernel,
                                                   (const size_t *)&s_openClKernelSize,
                                                   &ret);
    
    // Build the program
    ret = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
    ThrowOnError(ret);
    
    // Create the OpenCL kernel
    m_clKernel = clCreateKernel(program, "Convolution", &ret);

    ret = clReleaseProgram(program);
    ThrowOnError(ret);
    
    // load the source image to the gpu
    m_sourceImage = reinterpret_cast<float*>(GetSource().data);
    size_t sourceSize = GetSource().size().width * GetSource().size().height * sizeof(float);
    
    m_sourceMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                    sourceSize, NULL, &ret);
    m_resultMemoryObject = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY,
                                    sourceSize, NULL, &ret);
    
    ret = clEnqueueWriteBuffer(m_commandQueue, m_sourceMemoryObject, CL_TRUE, 0,
                               sourceSize, m_sourceImage, 0, NULL, NULL);
    ThrowOnError(ret);
    
    ret = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void *)&m_sourceMemoryObject);
    ThrowOnError(ret);
    
    ret = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void *)&m_resultMemoryObject);
    ThrowOnError(ret);
    
};

GpuConvolution::~GpuConvolution() {
    
    // We cant use the throw on error function here.
    // We should be logging an error here if something
    // happens.
    cl_int
    ret = clReleaseMemObject(m_sourceMemoryObject);
    ret = clReleaseMemObject(m_resultMemoryObject);
    
    ret = clFlush(m_commandQueue);
    ret = clFinish(m_commandQueue);
    ret = clReleaseKernel(m_clKernel);
    ret = clReleaseCommandQueue(m_commandQueue);
    ret = clReleaseContext(m_context);
    
}

void GpuConvolution::Convolve(const cv::Mat& kernel, cv::Mat& result) {
    
    cv::Size sourceSize = GetSource().size();
    const int sourceBufferSize = sourceSize.width * sourceSize.height;
    
    result = cv::Mat::zeros(sourceSize, CV_32F);
    size_t resultBufferSize = sourceBufferSize * sizeof(float);
    
    float *convolutionKernel = reinterpret_cast<float*>(kernel.data);
    size_t kernelBufferSize = kernel.size().width * kernel.size().height * sizeof(float);
    
    // Set up the memory objects
    cl_int ret = NULL;
    cl_mem kerelMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                      kernelBufferSize, NULL, &ret);
    
    // Copy the kernel data into the buffer
    ret = clEnqueueWriteBuffer(m_commandQueue, kerelMemoryObject, CL_TRUE, 0,
                               kernelBufferSize, convolutionKernel, 0, NULL, NULL);
    ThrowOnError(ret);
    
    // Set up the size data to be passed to the kernel
    size_t aSize = sizeof(int) * 4;
    int sizes[4] = {sourceSize.width, sourceSize.height,
                    kernel.size().width, kernel.size().height};
    
    cl_mem sizesMemoryObject = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY,
                                      aSize, NULL, &ret);
    ret = clEnqueueWriteBuffer(m_commandQueue, sizesMemoryObject, CL_TRUE, 0,
                               aSize, sizes, 0, NULL, NULL);
    ThrowOnError(ret);
    
    // Set the arguments of the kernel
    ret = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void *)&kerelMemoryObject);
    ThrowOnError(ret);
    
    ret = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void *)&sizesMemoryObject);
    ThrowOnError(ret);
    
    // Execute the OpenCL kernel on the source image
    size_t globalItemSize = sourceBufferSize; // Process the entire image
    size_t localItemSize = SetLocalItemSize(globalItemSize);
    
    ret = clEnqueueNDRangeKernel(m_commandQueue, m_clKernel, 1, NULL,
                                 &globalItemSize, &localItemSize, 0, NULL, NULL);
    ThrowOnError(ret);
    
    
    float *convolutionResult = reinterpret_cast<float*>(result.data);
    ret = clEnqueueReadBuffer(m_commandQueue, m_resultMemoryObject, CL_TRUE, 0,
                              resultBufferSize, convolutionResult, 0, NULL, NULL);
    ThrowOnError(ret);
    
    // These will leak if we throw earlier.
    // They should be wrapped in an RAII container
    // to be exception safe.
    ret = clReleaseMemObject(kerelMemoryObject);
    ThrowOnError(ret);
    ret = clReleaseMemObject(sizesMemoryObject);
    ThrowOnError(ret);
    
}

size_t GpuConvolution::SetLocalItemSize(size_t globalItemSize) {
    
    //Local item size must be divisible by the global item size
    
    if (globalItemSize % 64 == 0) {
        return 64;
    }
    else if (globalItemSize % 32 == 0) {
        return 32;
    }
    else if (globalItemSize % 16 == 0) {
        return 16;
    }
    else if (globalItemSize % 8 == 0) {
        return 8;
    }
    
    return 1;
    
}

