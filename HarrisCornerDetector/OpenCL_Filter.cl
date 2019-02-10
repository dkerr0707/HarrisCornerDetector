//
//  OpenCL_Filter.cl
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-09.
//  Copyright © 2019 David Kerr. All rights reserved.
//

__kernel void Convolution(__global const float *src, __global const float *convolutionKernel, __global float *result, __global int *sizes) {
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int sourceWidth = sizes[0];
    int sourceHeight = sizes[1];
    int kernelWidth = sizes[2];
    int kernelHeight = sizes[3];
    
    int row = i / sourceWidth;
    int col = i % sourceWidth;
    int rowOffset = kernelHeight / 2;
    int colOffset = kernelWidth / 2;
    
    result[i] = 0;
    
    // We are in the border so exit and leave the result as zero
    if (col < colOffset || col >= sourceWidth - colOffset ||
        row < rowOffset || row >= sourceHeight - rowOffset) {
        return;
    }
    
    // We can process this pixel
    for (int currentRow = -rowOffset; currentRow <= rowOffset; currentRow++) {
        for (int currentCol = -colOffset; currentCol <= colOffset; currentCol++) {
            
            // kernel space index
            int kernelRowIndex = currentRow + rowOffset;
            int kernelColIndex = currentCol + colOffset;
            
            // image space index
            int imageRowIndex = row + currentRow;
            int imageColIndex = col + currentCol;
            
            result[i] +=
                convolutionKernel[kernelWidth * kernelRowIndex + kernelColIndex] *
                src[sourceWidth * imageRowIndex + imageColIndex];
            
            
        }
    }

    
}
