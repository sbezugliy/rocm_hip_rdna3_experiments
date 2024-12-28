    // Constructor: initializes the neuron
    BaseNeuron::BaseNeuron(std::vector<float> inputs, size_t inputs_count=1, size_t outputs_count=1) {
        this->inputs_count = inputs_count;
        this->inputs = inputs;
    }

    // Activates main neuron function
    std::vector<float> BaseNeuron::activate() {
        return this->input;
    }

    // Defines neuron function derivative
    std::vector<float> BaseNeuron::derivative() {
        this->output[0] = 0
        return this->output;
    }