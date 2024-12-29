#include "BaseNeuron.hpp"

// Constructor: initializes the neuron
BaseNeuron::BaseNeuron(size_t inputs_count) : input(inputs_count), output(1) {
    // Initialize input and output vectors with zeros
    std::fill(input.begin(), input.end(), 0.0f);
    std::fill(output.begin(), output.end(), 0.0f);
}

// Activates main neuron function
std::vector<float> BaseNeuron::activate() {
    // Implement your activation logic here
    return input; // Return input for now
}

// Defines neuron function derivative
std::vector<float> BaseNeuron::derivative() {
    output[0] = 0.0f; // Example logic
    return output;
}

// Destructor: Cleans up resources if needed
BaseNeuron::~BaseNeuron() {
    // Destructor logic, if needed
}