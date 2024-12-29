# Specify the test programs
TESTS = run_tests
check_PROGRAMS = run_tests

# Source files for the test executable
run_tests_SOURCES = test_main.cpp neural/test_BaseNeuron.cpp

# Include directories
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/neural

# Compiler Flags
AM_CXXFLAGS = -g --offload-arch=gfx1036 --offload-arch=gfx1100

# Link with the necessary libraries, including GTest
# Place linker flags here
run_tests_LDADD = -lgtest -lgtest_main
run_tests_LDFLAGS = -pthread