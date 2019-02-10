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
    ret = clGetDeviceIDs( platformId, CL_DEVICE_TYPE_DEFAULT, 1,
                             &deviceId, &numDevices);
    
    // print device name
    size_t nameSize;
    clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &nameSize);
    m_deviceName = std::unique_ptr<char>(new char[nameSize]);
    clGetDeviceInfo(deviceId, CL_DEVICE_NAME, nameSize, m_deviceName.get(), NULL);
    std::cout << m_deviceName.get() << std::endl;
    
    m_context = clCreateContext( NULL, 1, &deviceId, NULL, NULL, &ret);
    m_command_queue = clCreateCommandQueue(m_context, deviceId, 0, &ret);
    
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(m_context, 1,
                                                   (const char **)&s_openClKernel,
                                                   (const size_t *)&s_openClKernelSize,
                                                   &ret);
    
    // Build the program
    ret = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
    
    // Create the OpenCL kernel
    m_clKernel = clCreateKernel(program, "Convolution", &ret);

    ret = clReleaseProgram(program);
    
    // load the source image to the gpu
    m_sourceImage = reinterpret_cast<float*>(GetSource().data);
    size_t sourceSize = GetSource().size().width * GetSource().size().height * sizeof(float);
    
    m_sourceMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                    sourceSize, NULL, &ret);
    m_resultMemoryObject = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY,
                                    sourceSize, NULL, &ret);
    
    ret = clEnqueueWriteBuffer(m_command_queue, m_sourceMemoryObject, CL_TRUE, 0,
                               sourceSize, m_sourceImage, 0, NULL, NULL);
    
    ret = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void *)&m_sourceMemoryObject);
    ret = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void *)&m_resultMemoryObject);
    
};

GpuConvolution::~GpuConvolution() {
    
    cl_int
    ret = clReleaseMemObject(m_sourceMemoryObject);
    ret = clReleaseMemObject(m_resultMemoryObject);
    
    ret = clFlush(m_command_queue);
    ret = clFinish(m_command_queue);
    ret = clReleaseKernel(m_clKernel);
    ret = clReleaseCommandQueue(m_command_queue);
    ret = clReleaseContext(m_context);
    
}

void GpuConvolution::Convolve(const cv::Mat& kernel, cv::Mat& result) {
    
    cv::Size sourceSize = GetSource().size();
    const int sourceBufferSize = sourceSize.width * sourceSize.height;
    
    result = cv::Mat::zeros(sourceSize, CV_32F);
    size_t resultBufferSize = sourceBufferSize * sizeof(float);
    
    size_t kernelBufferSize = kernel.size().width * kernel.size().height * sizeof(float);
    
    float *convolutionKernel = reinterpret_cast<float*>(kernel.data);

    
    // Set up the memory objects
    cl_int ret = NULL;
    cl_mem kerelMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                      kernelBufferSize, NULL, &ret);
    
    // Copy the kernel data into the buffer
    ret = clEnqueueWriteBuffer(m_command_queue, kerelMemoryObject, CL_TRUE, 0,
                               kernelBufferSize, convolutionKernel, 0, NULL, NULL);
    
    // Set up the size data to be passed to the kernel
    size_t aSize = sizeof(int) * 4;
    int sizes[4] = {sourceSize.width, sourceSize.height,
                    kernel.size().width, kernel.size().height};
    
    cl_mem sizesMemoryObject = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY,
                                      aSize, NULL, &ret);
    ret = clEnqueueWriteBuffer(m_command_queue, sizesMemoryObject, CL_TRUE, 0,
                               aSize, sizes, 0, NULL, NULL);
    
    // Set the arguments of the kernel
    ret = clSetKernelArg(m_clKernel, 1, sizeof(cl_mem), (void *)&kerelMemoryObject);
    ret = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void *)&sizesMemoryObject);
    
    // Execute the OpenCL kernel on the source image
    size_t global_item_size = sourceBufferSize; // Process the entire image
    size_t local_item_size = 1; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(m_command_queue, m_clKernel, 1, NULL,
                                 &global_item_size, &local_item_size, 0, NULL, NULL);
    
    
    float *convolutionResult = reinterpret_cast<float*>(result.data);
    ret = clEnqueueReadBuffer(m_command_queue, m_resultMemoryObject, CL_TRUE, 0,
                              resultBufferSize, convolutionResult, 0, NULL, NULL);
    
    ret = clReleaseMemObject(kerelMemoryObject);
    
    ret = clReleaseMemObject(sizesMemoryObject);
    
}
