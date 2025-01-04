// tests/test_BaseNeuron.cpp
#include <gtest/gtest.h>
#include "../../src/neural/BaseNeuron.hpp" // Adjust the path as needed based on your directory structure

template<typename T>
size_t sizeInBytes(size_t variable) {
    return variable * sizeof(T);
}

// Test correct initialization with default constructor
TEST(BaseNeuronTest, DefaultInitialization) {
    BaseNeuron neuron;

    auto result = neuron.activate();
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0.0f);
    
    auto derivative = neuron.derivative();
    EXPECT_EQ(derivative.size(), 1);
    EXPECT_EQ(derivative[0], 1.0f);
}

// Test correct initialization with specified inputs and outputs
TEST(BaseNeuronTest, CustomInitialization) {
    float inputs[2] = {0.5f, 0.8f};
    float outputs[2];
    BaseNeuron neuron(inputs, outputs, 2, 2);

    auto result = neuron.activate();
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0.5f);
    EXPECT_EQ(result[1], 0.8f);
    
    auto derivative = neuron.derivative();
    EXPECT_EQ(derivative.size(), 2);
    EXPECT_EQ(derivative[0], 1.0f);
    EXPECT_EQ(derivative[1], 1.0f);
}

// Test that activation with mismatched sizes throws an exception
TEST(BaseNeuronTest, MismatchedInitialization) {
    float inputs[2] = {0.5f, 0.8f};
    float outputs[1] = {1.0f};
    EXPECT_THROW(BaseNeuron(inputs, outputs, 2, 1), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}