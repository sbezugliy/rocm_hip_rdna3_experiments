// BaseNeuron.hpp
#ifndef BASE_NEURON_HPP
#define BASE_NEURON_HPP

#include <iostream>
#include <cstddef>
#include <vector>

class BaseNeuron {
public:
    // Constructor: initializes the neuron
    BaseNeuron(size_t inputs_count = 1);

    // Activates main neuron function
    std::vector<float> activate();

    // Defines neuron function derivative
    std::vector<float> derivative();

    // Destructor: Closes the connection if open
    ~BaseNeuron();

private:
    std::vector<float> input;
    std::vector<float> output;
};

#endif // BASE_NEURON_HPP