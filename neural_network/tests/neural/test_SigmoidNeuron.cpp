// tests/test_BaseNeuron.cpp
#include <gtest/gtest.h>
#include "../../src/neural/SigmoidNeuron.hpp" // Adjust the path as needed based on your directory structure

template<typename T>
size_t sizeInBytes(size_t variable) {
    return variable * sizeof(T);
}

// Test correct initialization with default constructor
TEST(SigmoidNeuronTest, DefaultInitialization) {
    SigmoidNeuron neuron;

    auto result = neuron.activate();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0.5f);
    
    auto derivative = neuron.derivative();
    EXPECT_EQ(derivative.size(), 1.0f);
    EXPECT_EQ(derivative[0], 0.25f);
}

// Test correct initialization with specified inputs and outputs
TEST(SigmoidNeuronTest, CustomInitialization) {
    float inputs[2] = {0.5f, 0.8f};
    float outputs[2];
    SigmoidNeuron neuron(inputs, outputs, 2, 2);

    auto result = neuron.activate();
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0.622459352f);
    EXPECT_EQ(result[1], 0.689974487f);
    
    auto derivative = neuron.derivative();
    EXPECT_EQ(derivative.size(), 2);
    EXPECT_EQ(derivative[0], 0.23500371f);
    EXPECT_EQ(derivative[1], 0.213909701f);
}

// Test that activation with mismatched sizes throws an exception
TEST(SigmoidNeuronTest, MismatchedInitialization) {
    float inputs[2] = {0.5f, 0.8f};
    float outputs[1] = {1.0f};
    EXPECT_THROW(SigmoidNeuron(inputs, outputs, 2, 1), std::runtime_error);
}