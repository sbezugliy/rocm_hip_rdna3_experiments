APP_NAME=matrices

# HIP Configuration
HIP_PATH?= $(wildcard /opt/rocm)
HIPCC=$(HIP_PATH)/bin/hipcc

# HIP Targets
HIPTARGETS=--amdgpu-target=gfx1100 # --amdgpu-target=gfx1036
CXXFLAGS = -g $(HIPTARGETS) 

# Matplot++ Library
MATPLOT_LIB_DIR=/usr/local/lib  # Path to where libmatplot.a is located

# Executable and Source Files
EXECUTABLE=./$(APP_NAME)
SOURCES=$(APP_NAME).cpp
OBJECTS=$(SOURCES:.cpp=.o)

CXX=$(HIPCC)

build: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ -I/usr/include -lrocrand

test: $(EXECUTABLE)
	$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
