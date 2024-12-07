#include <iostream>
#include <uv.h>
#include "kernel.hip.cpp" // Include the HIP kernel file

namespace uv;

// Callback function after writing to client
void on_write_end(uv_write_t* req, int status) {
    if (status != 0) {
        std::cerr << "Write error: " << uv_strerror(status) << std::endl;
    }
    free(req);
}

// Process data using HIP and sends back
void process_and_send(uv_stream_t* client, void* data, size_t size) {
    float* deviceData;

    hipMalloc(&deviceData, size * sizeof(float));
    hipMemcpy(deviceData, data, size * sizeof(float), hipMemcpyHostToDevice);
    
    // Launch HIP Kernel
    hipLaunchKernelGGL(hipKernel, dim3((size + 255) / 256), dim3(256), 0, 0, deviceData, size);
    hipDeviceSynchronize();
    
    hipMemcpy(data, deviceData, size * sizeof(float), hipMemcpyDeviceToHost);
    hipFree(deviceData);

    // Send processed data back to client
    uv_buf_t wrbuf = uv_buf_init(reinterpret_cast<char*>(data), size * sizeof(float));
    uv_write_t* write_req = (uv_write_t*) malloc(sizeof(uv_write_t));
    uv_write(write_req, client, &wrbuf, 1, on_write_end);
}

// Read callback invoked when data is received
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void read_cb(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        if (nread != UV_EOF) std::cerr << "Read error: " << uv_err_name(nread) << std::endl;
        uv_close((uv_handle_t*) client, nullptr);
        free(buf->base);
        return;
    }
    process_and_send(client, buf->base, nread / sizeof(float)); // Process data
    free(buf->base);
}

// Connection callback
void on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) {
        std::cerr << "New connection error: " << uv_strerror(status) << std::endl;
        return;
    }

    uv_tcp_t* client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);

    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, read_cb);
    } else {
        uv_close((uv_handle_t*) client, nullptr);
    }
}

int main() {
    // The XOR dataset
    float h_inputs[] = {0, 0, 1, 1, 0, 1, 1, 0};
    float h_labels[] = {0, 1, 1, 0};

    // Initialize weights and biases
    float h_weights1[] = {0.1f, 0.2f};
    float h_biases1[] = {0.1f, 0.1f};
    float h_weights2[] = {0.2f};
    float h_biases2[] = {0.1f};

    // Allocate device memory
    float *d_inputs, *d_weights1, *d_biases1, *d_weights2, *d_biases2;
    float *d_hidden_layer_output, *d_output, *d_labels;

    hipMalloc(&d_inputs, sizeof(h_inputs));

    hipMalloc(&d_weights1, sizeof(h_weights1));
    hipMalloc(&d_biases1, sizeof(h_biases1));
    
    hipMalloc(&d_weights2, sizeof(h_weights2));
    hipMalloc(&d_biases2, sizeof(h_biases2));
    
    hipMalloc(&d_hidden_layer_output, 2 * sizeof(float));
    
    hipMalloc(&d_output, 2 * sizeof(float));
    hipMalloc(&d_labels, sizeof(h_labels));

    // Copy data to the device
    hipMemcpy(d_inputs, h_inputs, sizeof(h_inputs), hipMemcpyHostToDevice);
    hipMemcpy(d_weights1, h_weights1, sizeof(h_weights1), hipMemcpyHostToDevice);
    hipMemcpy(d_biases1, h_biases1, sizeof(h_biases1), hipMemcpyHostToDevice);
    hipMemcpy(d_weights2, h_weights2, sizeof(h_weights2), hipMemcpyHostToDevice);
    hipMemcpy(d_biases2, h_biases2, sizeof(h_biases2), hipMemcpyHostToDevice);
    hipMemcpy(d_labels, h_labels, sizeof(h_labels), hipMemcpyHostToDevice);

    const float learning_rate = 0.5f;
    const int num_epochs = 1000;
    const int num_inputs = 4;

    // Training loop
    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        for (int i = 0; i < num_inputs; i += 2) {
            // Launch kernel to perform forward pass
            hipLaunchKernelGGL(forward, 1, 2, 0, 0, d_inputs + i, d_weights1, d_biases1, d_weights2, d_biases2, d_hidden_layer_output, d_output, 2);
            // Sync before backward pass
            hipDeviceSynchronize();
            // Launch kernel to perform backward pass
            hipLaunchKernelGGL(backward, 1, 2, 0, 0, d_inputs + i, d_weights1, d_biases1, d_weights2, d_biases2, d_hidden_layer_output, d_output, d_labels + (i / 2), learning_rate, 2);
            // Sync
            hipDeviceSynchronize();
        }
    }

    // Copy the result from device back to host
    hipMemcpy(h_weights1, d_weights1, sizeof(h_weights1), hipMemcpyDeviceToHost);
    hipMemcpy(h_weights2, d_weights2, sizeof(h_weights2), hipMemcpyDeviceToHost);

    std::cout << "Trained weights (Layer 1): " << h_weights1[0] << ", " << h_weights1[1] << std::endl;
    std::cout << "Trained weights (Layer 2): " << h_weights2[0] << std::endl;

    // Free device memory
    hipFree(d_inputs);
    hipFree(d_weights1);
    hipFree(d_biases1);
    hipFree(d_weights2);
    hipFree(d_biases2);
    hipFree(d_hidden_layer_output);
    hipFree(d_output);
    hipFree(d_labels);
    
    uv_loop_t* loop = uv_default_loop();
    uv_tcp_t server1, server2;

    // Initialize TCP endpoints
    uv_tcp_init(loop, &server1);
    uv_tcp_bind(&server1, uv_ip4_addr("0.0.0.0", 7000));
    uv_listen((uv_stream_t*)&server1, 128, on_new_connection);

    uv_tcp_init(loop, &server2);
    uv_tcp_bind(&server2, uv_ip4_addr("0.0.0.0", 7001));
    uv_listen((uv_stream_t*)&server2, 128, on_new_connection);

    // Run the libuv event loop
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);

    return 0;
}
