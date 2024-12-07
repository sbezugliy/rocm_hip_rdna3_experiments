cmake_minimum_required(VERSION 3.10)
   project(NN)

   # Find Matplot++
   find_package(Matplot++ REQUIRED)
   find_package(hip REQUIRED)
   include_directories(/opt/rocm/include)
   
   # Add HIP as a language
   enable_language(HIP)
   set(CMAKE_HIP_COMPILER /opt/rocm/bin/hipcc) 
   
   # Specify the source files
   add_executable(nn nn.cpp kernel.hip.cpp)

   # Link Matplot++ to your project
   target_link_libraries(nn PRIVATE Matplot++::matplot)

   # Add HIP specific settings
   set_target_properties(nn PROPERTIES
       HIP_ARCH GFX1100
   )