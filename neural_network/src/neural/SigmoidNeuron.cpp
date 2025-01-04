#include "SigmoidNeuron.hpp"

SigmoidNeuron::SigmoidNeuron(float* input_ptr, float* output_ptr, size_t inputs_size, size_t outputs_size)
    : BaseNeuron(input_ptr, output_ptr, inputs_size, outputs_size) {}

// Activate function
std::vector<float> SigmoidNeuron::activate() {
    std::vector<float> activations(inputs_size);
    for (size_t i = 0; i < inputs_size; ++i) {
        float val = 1.0f / (1.0f + std::exp(-input[i])); // Sigmoid function
        if (output != nullptr && i < outputs_size) {
            output[i] = val;
        }
        activations[i] = val;
    }
    return activations;
}

// Derivative function
std::vector<float> SigmoidNeuron::derivative() {
    std::vector<float> derivatives(inputs_size);
    for (size_t i = 0; i < inputs_size; ++i) {
        float sigmoid_val = 1.0f / (1.0f + std::exp(-input[i]));
        derivatives[i] = sigmoid_val * (1 - sigmoid_val); // Derivative of sigmoid
    }
    return derivatives;
}

// Destructor
SigmoidNeuron::~SigmoidNeuron() {}