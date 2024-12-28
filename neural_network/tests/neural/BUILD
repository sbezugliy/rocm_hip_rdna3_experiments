cc_test(
    name = "test_BaseNeuron",
    srcs = ["test_BaseNeuron.cpp"],
    deps = [
        "//neural:neural_lib",
        "//tcp_client:tcp_client_lib",
        # Add the dependency for Catch2 if needed
    ],
    copts = ["--amdgpu-target=gfx1100"],
)