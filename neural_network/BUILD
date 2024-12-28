cc_binary(
    name = "neural_network",
    srcs = [
        "neural_network.cpp",
        "hip_session.cpp",
    ],
    deps = [
        "//neural:neural_lib",
        "//tcp_client:tcp_client_lib",
    ],
    copts = ["--amdgpu-target=gfx1100"],
    linkopts = ["-L/opt/rocm/lib", "-lrocrand", "-lyaml-cpp", "-limgui", "-lSDL3"],
)