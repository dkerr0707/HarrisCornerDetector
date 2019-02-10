//
//  GpuConvolution.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "GpuConvolution.hpp"

#include <iostream>

GpuConvolution::GpuConvolution(cv::Mat src) : Convolution(src) {
    
    // The OpenCL set up code was modified from the following source:
    // https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
    
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1,
                             &device_id, &ret_num_devices);
        
    m_context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    m_command_queue = clCreateCommandQueue(m_context, device_id, 0, &ret);
    
    
    int MAX_SOURCE_SIZE (0x100000);
    
    // Load the kernel source code into the array source_str
    FILE *fp;
    char* source_str;
    size_t source_size;
    
    fp = fopen("/Users/dkerr/dev/HarrisCornerDetector/HarrisCornerDetector/OpenCL_Filter.cl", "r");
    if (!fp) {
        throw std::runtime_error("Failed to load OpenCL kernel");
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
    
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(m_context, 1,
                                                   (const char **)&source_str,
                                                   (const size_t *)&source_size,
                                                   &ret);
    
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    
    // Create the OpenCL kernel
    m_clKernel = clCreateKernel(program, "Convolution", &ret);

    ret = clReleaseProgram(program);
    
    // load the source image to the gpu
    m_sourceImage = reinterpret_cast<float*>(GetSource().data);
    size_t sourceSize = GetSource().size().width * GetSource().size().height * sizeof(float);
    
    m_sourceMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                      sourceSize, NULL, &ret);
    
    ret = clEnqueueWriteBuffer(m_command_queue, m_sourceMemoryObject, CL_TRUE, 0,
                               sourceSize, m_sourceImage, 0, NULL, NULL);
    
    ret = clSetKernelArg(m_clKernel, 0, sizeof(cl_mem), (void *)&m_sourceMemoryObject);
    
};

GpuConvolution::~GpuConvolution() {
    
    cl_int ret = clReleaseMemObject(m_sourceMemoryObject);
    
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
    size_t resultBufferSize = result.size().width * result.size().height * sizeof(float);
    
    size_t kernelBufferSize = kernel.size().width * kernel.size().height * sizeof(float);
    
    float *convolutionKernel = reinterpret_cast<float*>(kernel.data);

    
    // Set up the memory objects
    cl_int ret = NULL;
    cl_mem kerelMemoryObject = clCreateBuffer(m_context, CL_MEM_READ_ONLY,
                                      kernelBufferSize, NULL, &ret);

    cl_mem resultMemoryObject = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY,
                                      resultBufferSize, NULL, &ret);
    
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
    ret = clSetKernelArg(m_clKernel, 2, sizeof(cl_mem), (void *)&resultMemoryObject);
    ret = clSetKernelArg(m_clKernel, 3, sizeof(cl_mem), (void *)&sizesMemoryObject);
    
    // Execute the OpenCL kernel on the source image
    size_t global_item_size = sourceBufferSize; // Process the entire image
    size_t local_item_size = 1; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(m_command_queue, m_clKernel, 1, NULL,
                                 &global_item_size, &local_item_size, 0, NULL, NULL);
    
    
    float *convolutionResult = reinterpret_cast<float*>(result.data);
    ret = clEnqueueReadBuffer(m_command_queue, resultMemoryObject, CL_TRUE, 0,
                              resultBufferSize, convolutionResult, 0, NULL, NULL);
    
    ret = clReleaseMemObject(kerelMemoryObject);
    ret = clReleaseMemObject(resultMemoryObject);
    ret = clReleaseMemObject(sizesMemoryObject);
    
}
