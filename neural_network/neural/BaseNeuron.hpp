// BaseNeuron.h
#ifndef BASE_NEURON_H
#define BASE_NEURON_H

class BaseNeuron {
public:
    // Constructor: initializes the neuron
    BaseNeuron(size_t inputs_count=1);

    // Activates main neuron function
    void activate();

    // Defines neuron function derivative
    void derivative();

    // Destructor: Closes the connection if open
    ~BaseNeuron();
    std::vector<float> output;

private:
    std::vector<float> input(inputs_count);
};

#endif // BASE_NEURON_H