// BaseNeuron.hpp
#ifndef SIGMOID_NEURON_HPP
#define SIGMOID_NEURON_HPP

#include <iostream>
#include <vector>
#include <stdexcept>

class SgmoidNeuron : BaseNeuron {
public:
    // Constructor: initializes the neuron
    SigmoidNeuron(float* input_ptr = nullptr, float* output_ptr = nullptr, 
               size_t inputs_size = 1, size_t outputs_size = 1);

    // Activates main neuron function
    std::vector<float> activate();

    // Defines neuron function derivative
    std::vector<float> derivative();

    // Destructor
    ~BaseNeuron();

private:
    float* input;
    float* output;
    size_t inputs_size;
    size_t outputs_size;

    void check_and_allocate();
};

#endif // BASE_NEURON_HPP