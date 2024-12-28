toolchain {
     toolchain_identifier: "rocm_hipcc_toolchain"
     abi_version: "local"
     abi_libc_version: "local"
     builtin_sysroot: ""
     cc_target_os: "local"

     tool_path { name: "gcc" path: "/opt/rocm/bin/hipcc" }
     tool_path { name: "g++" path: "/opt/rocm/bin/hipcc" }
     tool_path { name: "ar" path: "/usr/bin/ar" }
     tool_path { name: "strip" path: "/usr/bin/strip" }
     tool_path { name: "ld" path: "/usr/bin/ld" }
     tool_path { name: "gcov" path: "/usr/bin/gcov" }

     # This is where you translate Bazel's idea of
     # "compilation" into a command line invocation of
     # /opt/rocm/bin/hipcc.
     action_config {
       config_name: "c++-compile"
       action_name: "c++-compile"
       tool { tool_path: "/opt/rocm/bin/hipcc" }
       flag_set { action: "c++-compile" flag_group { flag: "-c" } }
     }

     # Additional compiler flags
     # Modify with the flags you need for your project
     cxx_flag: "--amdgpu-target=gfx1100" # Example HIP target
     cxx_flag: "-std=c++14" # Or whichever standard you're using
   }