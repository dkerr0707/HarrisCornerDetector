__kernel void vector_add(__global const float *src, __global const float *convolutionKernel, __global float *result, __global int *sizes) {
    
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int sourceWidth = sizes[0];
    int sourceHeight = sizes[1];
    int kernelWidth = sizes[2];
    int kernelHeight = sizes[3];
    
    // Do the operation
    result[i] = kernelWidth;//src[i] + convolutionKernel[i];
}
