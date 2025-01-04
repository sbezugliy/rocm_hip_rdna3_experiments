#ifndef BASE_NEURON_HPP
#define BASE_NEURON_HPP

#include <vector>

class BaseNeuron {
public:
    BaseNeuron(float* input_ptr = nullptr, float* output_ptr = nullptr, 
               size_t inputs_size = 1, size_t outputs_size = 1);

    std::vector<float> activate();
    std::vector<float> derivative();

    virtual ~BaseNeuron();

    // Accessor methods
    float* getInput() const { return input; }
    float* getOutput() const { return output; }
    size_t getInputsSize() const { return inputs_size; }
    size_t getOutputsSize() const { return outputs_size; }

protected:
    float* input;
    float* output;
    size_t inputs_size;
    size_t outputs_size;

    void check_and_allocate();

private:
    bool input_managed;  // True if input is allocated internally
    bool output_managed; // True if output is allocated internally
};

#endif // BASE_NEURON_HPP