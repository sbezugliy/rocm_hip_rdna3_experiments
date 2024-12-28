APP_NAME=matrices

# HIP Configuration
HIP_PATH?= $(wildcard /opt/rocm)
HIPCC=$(HIP_PATH)/bin/hipcc

# HIP Targets
HIPTARGETS=--amdgpu-target=gfx1100 # --amdgpu-target=gfx1036
CXXFLAGS = -g $(HIPTARGETS) -I/usr/include -I../tcp_client

# Executable and Source Files
EXECUTABLE=./$(APP_NAME)
SOURCES=$(APP_NAME).cpp ../tcp_client/TcpClient.cpp
OBJECTS=$(SOURCES:.cpp=.o)

CXX=$(HIPCC)

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

test: $(EXECUTABLE)
	$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
