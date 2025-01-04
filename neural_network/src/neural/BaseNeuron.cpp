#include "BaseNeuron.hpp"
#include <algorithm> // For std::fill_n
#include <cstring>   // For std::memcpy

BaseNeuron::BaseNeuron(float* input_ptr, float* output_ptr, size_t inputs_size, size_t outputs_size)
    : inputs_size(inputs_size), outputs_size(outputs_size), input(input_ptr), output(output_ptr), input_managed(input_ptr == nullptr), output_managed(output_ptr == nullptr) {
    
    if (inputs_size != outputs_size) {
        throw std::runtime_error("Input and output sizes must match.");
    }
    // Allocate memory for inputs and outputs if not provided
    if (input_managed) {
        input = new float[inputs_size];
        std::fill_n(input, inputs_size, 0.0f);
    }
    if (output_managed) {
        output = new float[outputs_size];
        std::fill_n(output, outputs_size, 0.0f);
    }
}

std::vector<float> BaseNeuron::activate() {
    std::memcpy(output, input, inputs_size * sizeof(float));
    return std::vector<float>(output, output + outputs_size);
}

std::vector<float> BaseNeuron::derivative() {
    return std::vector<float>(inputs_size, 1.0f);
}

BaseNeuron::~BaseNeuron() {
    // Only release memory if it was allocated by this instance
    if (input_managed) {
        delete[] input;
    }
    if (output_managed) {
        delete[] output;
    }
}