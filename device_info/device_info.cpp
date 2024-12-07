#include <iostream>
#include <hip/hip_runtime.h>

int main() {
    int deviceCount;
    hipError_t err = hipGetDeviceCount(&deviceCount);
    if (err != hipSuccess) {
        std::cerr << "Failed to get the number of devices: " << hipGetErrorString(err) << std::endl;
        return 1;
    }

    for (int device = 0; device < deviceCount; ++device) {
        hipDeviceProp_t prop;
        err = hipGetDeviceProperties(&prop, device);
        if (err != hipSuccess) {
            std::cerr << "Failed to get device properties: " << hipGetErrorString(err) << std::endl;
            continue;
        }

        std::cout << "Device " << device << ": " << prop.name << std::endl;
        std::cout << "  Memory Clock Rate (MHz): " << prop.memoryClockRate / 1024 << std::endl;
        std::cout << "  Memory Bus Width (bits): " << prop.memoryBusWidth << std::endl;
        std::cout << "  Peak Memory Bandwidth (GB/s): "
                  << 2.0 * prop.memoryClockRate * (prop.memoryBusWidth / 8) / 1.0e6 << std::endl;
        std::cout << "  Total Global Memory (GB): " << prop.totalGlobalMem / (1024 * 1024 * 1024) << std::endl;
        std::cout << "  Shared Memory Per Block (KB): " << prop.sharedMemPerBlock / 1024.0 << std::endl;
        std::cout << "  Number of Multiprocessors: " << prop.multiProcessorCount << std::endl;
        std::cout << "  Maximum Threads Per Block: " << prop.maxThreadsPerBlock << std::endl;
        std::cout << "  Maximum Threads Per Multiprocessor: " << prop.maxThreadsPerMultiProcessor << std::endl;
        std::cout << "  Warp Size: " << prop.warpSize << std::endl;
        std::cout << "  Address Capability Supported: " 
                  << (prop.unifiedAddressing ? "Unified Addressing supported" : "Separate Address Spaces") << std::endl;
        std::cout << "  Can Access Host Memory Directly: " 
                  << (prop.directManagedMemAccessFromHost ? "Yes" : "No") << std::endl;

        // Simulate memory allocation on the GPU and print addresses
        size_t bufferSize = 1024 * 1024; // 1 MB buffer size
        void* deviceBuffer;
        err = hipMalloc(&deviceBuffer, bufferSize);
        if (err == hipSuccess) {
            std::cout << "  Buffer allocated at device address: " << deviceBuffer << std::endl;
            hipFree(deviceBuffer);
        } else {
            std::cerr << "  Failed to allocate buffer on the device: " << hipGetErrorString(err) << std::endl;
        }

        // Simulate managed memory allocation
        void* managedBuffer;
        err = hipMallocManaged(&managedBuffer, bufferSize);
        if (err == hipSuccess) {
            std::cout << "  Managed buffer allocated at address: " << managedBuffer << std::endl;
            hipFree(managedBuffer);
        } else {
            std::cerr << "  Failed to allocate managed buffer: " << hipGetErrorString(err) << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}