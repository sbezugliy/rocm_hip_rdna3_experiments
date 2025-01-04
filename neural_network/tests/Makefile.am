TESTS = run_tests
check_PROGRAMS = run_tests

run_tests_SOURCES = test_neural_network.cpp neural/test_BaseNeuron.cpp neural/test_SigmoidNeuron.cpp

# Include directories
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/neural

# Compiler Flags
AM_CXXFLAGS = -g --offload-arch=gfx1036 --offload-arch=gfx1100

# Link with the necessary libraries, including GTest
run_tests_LDADD = -lgtest -lgtest_main -L$(top_builddir)/src ../src/libneural_network.a
run_tests_LDFLAGS = -pthread -L/opt/rocm/lib -lSDL3 -limgui -lyaml-cpp -lrocrand 

TESTS_ENVIRONMENT = ./run_tests --gtest_output=all