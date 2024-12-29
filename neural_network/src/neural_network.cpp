#include <iostream>
#include <hip/hip_runtime.h>
#include "vector_multiply_kernel.hip.cpp" // Include the HIP kernel file
#include <rocrand/rocrand.h>
#include <time.h>
#include <cstddef> 
#include <yaml-cpp/yaml.h>
#include "tcp_client/TcpClient.hpp"

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

void tcp_server(std::string server_host, int server_port){
    TcpClient client(server_host, server_port);
    if (client.connectToServer()) {
        client.startCommunication();
    }
}

int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");

    size_t block_size = config["hip"]["block_size"].as<size_t>();
    size_t vector_b_size = config["vector_ops"]["vector_b_size"].as<size_t>();
    size_t vector_size = config["vector_ops"]["vector_size"].as<size_t>();
    
    std::string tcp_server_host = config["tcp_server"]["host"].as<std::string>();
    int tcp_server_port = config["tcp_server"]["port"].as<int>();

    rocrand_generator gen;
    std::vector<float> h_vector_a(vector_size, 0.0f);
    std::vector<float> h_vector_output(vector_size, 0.0f); 

    // Extract the float array from the YAML
    std::vector<float> h_vector_b;
    for (const auto& value : config["vector_ops"]["vector_b"]) {
        h_vector_b.push_back(value.as<float>());
    }

    float *d_vector_a, *d_vector_output;

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
    hipMemcpy(h_vector_a.data(), d_vector_a, sizeInBytes<float>(vector_size), hipMemcpyDeviceToHost);


    hipMalloc((void **)&d_vector_b, sizeInBytes<float>(vector_b_size));
    hipMemcpy(d_vector_b, h_vector_b.data(), sizeInBytes<float>(vector_b_size), hipMemcpyHostToDevice);

    hipMalloc((void **)&d_vector_output, sizeInBytes<float>(vector_size));
    hipMemcpy(d_vector_output, h_vector_output.data(), sizeInBytes<float>(vector_size), hipMemcpyHostToDevice);
    
    hipLaunchKernelGGL(multiply_vector, dim3(num_blocks), dim3(block_size), 0, 0, d_vector_a, d_vector_b, d_vector_output, vector_size, vector_b_size);
    hipDeviceSynchronize();
    
    hipMemcpy(h_vector_output.data(), d_vector_output, sizeInBytes<float>(vector_size), hipMemcpyDeviceToHost);

    print_vector("Vector b", h_vector_b.data(), vector_b_size);
    print_vector("Vector a", h_vector_a.data(), vector_size);
    print_vector("Result", h_vector_output.data(), vector_size);

    tcp_server(tcp_server_host, tcp_server_port);

    rocrand_destroy_generator(gen);

    hipFree(d_vector_a);
    hipFree(d_vector_b);
    hipFree(d_vector_output);
    return 0;
}
