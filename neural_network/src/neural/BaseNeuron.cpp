// BaseNeuron.cpp
#include "BaseNeuron.hpp"
#include <cstring> // For std::memcpy

BaseNeuron::BaseNeuron(float* input_ptr, float* output_ptr, size_t inputs_size, size_t outputs_size)
    : inputs_size(inputs_size), outputs_size(outputs_size) {
    
    // Allocate memory for inputs and outputs if not provided
    input = input_ptr ? input_ptr : new float[inputs_size];
    output = output_ptr ? output_ptr : new float[outputs_size];

    // Initialize allocated arrays to zero if newly allocated
    if (!input_ptr) {
        std::fill_n(input, inputs_size, 0.0f);
    }
    if (!output_ptr) {
        std::fill_n(output, outputs_size, 0.0f);
    }
}

std::vector<float> BaseNeuron::activate() {
    if (inputs_size != outputs_size) {
        throw std::runtime_error("Input and output sizes must match for this simple passthrough");
    }

    std::memcpy(output, input, inputs_size * sizeof(float));
    return std::vector<float>(output, output + outputs_size);
}

std::vector<float> BaseNeuron::derivative() {
    return std::vector<float>(inputs_size, 1.0f);
}

BaseNeuron::~BaseNeuron() {
    // Only release memory if it was allocated by this instance
    // delete[] input;
    // delete[] output;
}