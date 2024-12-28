  package(default_visibility = ["//visibility:public"])

   filegroup(
       name = "empty",
       srcs = [],
   )

   cc_toolchain_suite(
       name = "toolchain",
       toolchains = {
           "local": ":cc-compiler",
       },
   )

   cc_toolchain(
       name = "cc-compiler",
       toolchain_identifier = "rocm_hipcc_toolchain",
       all_files = ":empty",
       ar_files = ":empty",
       as_files = ":empty",
       compiler_files = ":empty",
       dwp_files = ":empty",
       linker_files = ":empty",
       strip_files = ":empty",
       objcopy_files = ":empty",
       cpu = "local",
       toolchain_config = "//tools/rocm:rocm_toolchain_config",
   )