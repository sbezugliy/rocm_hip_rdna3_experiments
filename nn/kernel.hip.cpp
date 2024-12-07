#include <hip/hip_runtime.h>
#include <cmath>

#define THREADS_PER_BLOCK 4

__device__ float sigmoid(float x) {
    return 1.0f / (1.0f + expf(-x));
}

__device__ float sigmoid_derivative(float x) {
    return x * (1.0f - x);
}

__global__ void forward(float* inputs, float* weights1, float* biases1, float* weights2, float* biases2, float* hidden_layer_output, float* output, int num_inputs) {
    int idx = threadIdx.x;
    if (idx < num_inputs) {
        hidden_layer_output[idx] = sigmoid(inputs[idx] * weights1[0] + inputs[1 - idx] * weights1[1] + biases1[idx]);
        output[idx] = sigmoid(hidden_layer_output[idx] * weights2[0] + biases2[idx]);
    }
}

__global__ void backward(float* inputs, float* weights1, float* biases1, float* weights2, float* biases2, float* hidden_layer_output, float* output, float* expected_output, float learning_rate, int num_inputs) {
    int idx = threadIdx.x;
    if (idx < num_inputs) {
        float error = expected_output[idx] - output[idx];
        float d_output = error * sigmoid_derivative(output[idx]);

        float error_hidden = d_output * weights2[0];
        float d_hidden = error_hidden * sigmoid_derivative(hidden_layer_output[idx]);

        weights2[0] += learning_rate * d_output * hidden_layer_output[idx];
        biases2[idx] += learning_rate * d_output;

        weights1[idx] += learning_rate * d_hidden * inputs[0];
        weights1[1 - idx] += learning_rate * d_hidden * inputs[1];
        biases1[idx] += learning_rate * d_hidden;
    }
}