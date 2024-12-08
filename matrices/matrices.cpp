#include <iostream>
#include <hip/hip_runtime.h>
#include "vector_multiply_kernel.hip.cpp" // Include the HIP kernel file
#include <rocrand/rocrand.h>
#include <time.h>
#include <TcpServer.h>
#include <cstddef> 
#include <yaml-cpp/yaml.h>

template<typename T>
size_t sizeInBytes(size_t variable) {
    return variable * sizeof(T);
}

template<typename IntegerType>
size_t numBlocks(IntegerType variable, IntegerType block_size) {
    return (variable + block_size - 1) / block_size;
}

void print_vector(char *title, float *vector, size_t vector_size) {
    std::cout << title << "<" << vector_size << ">: [ ";
    for (size_t i = 0; i < vector_size; ++i) {
        std::cout << vector[i];
        if (i < vector_size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

void print_stat(char *title, int variable) {
    std::cout << title << variable << std::endl;
}

int main() {
    size_t vector_size = 100;
    size_t vector_b_size = 5;
    size_t block_size = 256;

    rocrand_generator gen;
    float *d_vector_a, *h_vector_a, *h_vector_output, *d_vector_output;

    h_vector_a = (float*)malloc(sizeInBytes<float>(vector_size));
    h_vector_output = (float*)malloc(sizeInBytes<float>(vector_size));

    float h_vector_b[] = {-0.2575478, 0.765, 0.235346, -0.2357, 0.134537};
    
    size_t num_blocks = numBlocks(vector_size, block_size);

    float *d_vector_b;

    print_stat("Main vector size: ", vector_size);
    print_stat("Second vector size: ", vector_b_size);
    print_stat("Blocks number: ", num_blocks);
    print_stat("Block size: ", block_size);

    hipMalloc((void **)&d_vector_a, sizeInBytes<float>(vector_size));
    rocrand_create_generator(&gen, ROCRAND_RNG_PSEUDO_DEFAULT);
    rocrand_set_seed(gen, time(NULL));
    rocrand_generate_uniform(gen, d_vector_a, vector_size);
    hipMemcpy(h_vector_a, d_vector_a, sizeInBytes<float>(vector_size), hipMemcpyDeviceToHost);


    hipMalloc((void **)&d_vector_b, sizeInBytes<float>(vector_b_size));
    hipMemcpy(d_vector_b, h_vector_b, sizeInBytes<float>(vector_b_size), hipMemcpyHostToDevice);

    hipMalloc((void **)&d_vector_output, sizeInBytes<float>(vector_size));
    hipMemcpy(d_vector_output, h_vector_output, sizeInBytes<float>(vector_size), hipMemcpyHostToDevice);
    
    hipLaunchKernelGGL(multiply_vector, dim3(num_blocks), dim3(block_size), 0, 0, d_vector_a, d_vector_b, d_vector_output, vector_size, vector_b_size);
    hipDeviceSynchronize();
    
    hipMemcpy(h_vector_output, d_vector_output, sizeInBytes<float>(vector_size), hipMemcpyDeviceToHost);

    print_vector("Vector b", h_vector_b, vector_b_size);
    print_vector("Vector a", h_vector_a, vector_size);
    print_vector("Result", h_vector_output, vector_size);

    rocrand_destroy_generator(gen);
    hipFree(d_vector_a);
    hipFree(d_vector_b);
    hipFree(d_vector_output);

    return 0;
}
