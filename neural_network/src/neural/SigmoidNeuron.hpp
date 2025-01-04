#ifndef SIGMOID_NEURON_HPP
#define SIGMOID_NEURON_HPP

#include "BaseNeuron.hpp"
#include <vector>
#include <cmath>

class SigmoidNeuron : public BaseNeuron {
public:
    SigmoidNeuron(float* input_ptr = nullptr, float* output_ptr = nullptr, 
                  size_t inputs_size = 1, size_t outputs_size = 1);

    std::vector<float> activate();
    std::vector<float> derivative();

    ~SigmoidNeuron();
};

#endif // SIGMOID_NEURON_HPP