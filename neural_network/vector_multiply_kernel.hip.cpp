#include <hip/hip_runtime.h>
#include <cmath>
#include <stdlib.h>

#define THREADS_PER_BLOCK 256

//ToDo: Update to use for changing vector_b
__device__ float multiply_element(float element_a, float element_b) {
    return element_a * element_b;
}

__device__ float multiply(float vector_a_element, float* vector_b, size_t vector_b_size) {
    float acc = 0;
    for(size_t i = 0; i < vector_b_size; ++i) {
        acc += multiply_element(vector_a_element, vector_b[i]);
    }
    return acc;
}

__global__ void multiply_vector(float* d_vector_a, float* d_vector_b, float* d_vector_output, size_t vector_size, size_t vector_b_size ) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < vector_size) {
        d_vector_output[idx] = multiply(d_vector_a[idx], d_vector_b, vector_b_size);
    }
}

// __global__ void generate_vector(float* vector, size_t vector_size, rocrand_status* states) {
//     int idx = blockIdx.x * blockDim.x + threadIdx.x;
//     if (idx < vector_size) {
//         vector[idx] = rans;
//     }
// }
