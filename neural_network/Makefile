APP_NAME=neural_network

# HIP Configuration
HIP_PATH?= $(wildcard /opt/rocm)
HIPCC=$(HIP_PATH)/bin/hipcc

# HIP Targets
HIPTARGETS=--amdgpu-target=gfx1100 # --amdgpu-target=gfx1036
CXXFLAGS = -g $(HIPTARGETS) -I/usr/include -I../tcp_client -I./neural

# Executable and Source Files
EXECUTABLE=./$(APP_NAME)
SOURCES=$(APP_NAME).cpp \
	../tcp_client/TcpClient.cpp \
	./hip_session.cpp \
	./neural/BaseNeuron.cpp \
	./neural/Neuron.cpp
OBJECTS=$(SOURCES:.cpp=.o)

CXX=$(HIPCC)

# Test sources
TEST_SOURCES=tests/test_main.cpp tests/neural/test_BaseNeuron.cpp
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
TEST_EXECUTABLE=tests/run_tests

build: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ \
	-L/opt/rocm/lib -lrocrand \
	-I/usr/include \
	-lyaml-cpp \
	-limgui \
	-lSDL3

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


test: $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -o $(TEST_EXECUTABLE) $(LDFLAGS)
	$(TEST_EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f $(TEST_EXECUTABLE)
	rm -f $(TEST_OBJECTS)