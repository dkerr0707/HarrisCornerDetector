//
//  OpenCL_Filer.h
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-10.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// To simplify the loading requirments for the GPU
// implementation I decided to copy the kernel to
// a header file and load it from here. The other
// option is to load the .cl file into the application
// directly. Because I could not guarantee a full
// reference path on other systems I felt this
// is the least error prone way to deal with the
// OpenCL kenel for this application.

#pragma once

static size_t s_openClKernelSize = 1024*1024;
static char* s_openClKernel =

"__kernel void Convolution(__global const float *src, __global const float *convolutionKernel, __global float *result, __global int *sizes) {\n"
    
    "// Get the index of the current element to be processed\n"
    "int i = get_global_id(0);\n"
    "int sourceWidth = sizes[0];\n"
    "int sourceHeight = sizes[1];\n"
    "int kernelWidth = sizes[2];\n"
    "int kernelHeight = sizes[3];\n"
    
    "int row = i / sourceWidth;\n"
    "int col = i % sourceWidth;\n"
    "int rowOffset = kernelHeight / 2;\n"
    "int colOffset = kernelWidth / 2;\n"
    
    "result[i] = 0;\n"
    
    "// We are in the border so exit and leave the result as zero\n"
    "if (col < colOffset || col >= sourceWidth - colOffset ||\n"
        "row < rowOffset || row >= sourceHeight - rowOffset) {\n"
        "return;\n"
    "}"
    
    "// We can process this pixel\n"
    "for (int currentRow = -rowOffset; currentRow <= rowOffset; currentRow++) {\n"
        "for (int currentCol = -colOffset; currentCol <= colOffset; currentCol++) {\n"
            
            "// kernel space index\n"
            "int kernelRowIndex = currentRow + rowOffset;\n"
            "int kernelColIndex = currentCol + colOffset;\n"
            
            "// image space index\n"
            "int imageRowIndex = row + currentRow;\n"
            "int imageColIndex = col + currentCol;\n"
            
            "result[i] +=\n"
            "convolutionKernel[kernelWidth * kernelRowIndex + kernelColIndex] *\n"
            "src[sourceWidth * imageRowIndex + imageColIndex];\n"
            
            
        "}\n"
    "}\n"
"}\n";
