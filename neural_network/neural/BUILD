cc_library(
    name = "neural_lib",
    srcs = [
        "BaseNeuron.cpp",
        "Neuron.cpp",
    ],
    hdrs = [
        # List header files if any, e.g. "BaseNeuron.h",
    ],
    includes = ["."],
    copts = ["--amdgpu-target=gfx1100"],
    visibility = ["//visibility:public"],
)