APP_NAME=device_info

HIP_PATH?= $(wildcard /opt/rocm)
HIPCC=$(HIP_PATH)/bin/hipcc


EXECUTABLE=./$(APP_NAME)
SOURCES=$(EXECUTABLE).cpp
OBJECTS=$(SOURCES:.cpp=.o)


build: $(SOURCES) test

CXXFLAGS =-g
CXX=$(HIPCC)


$(EXECUTABLE): $(OBJECTS)
	$(HIPCC) $(OBJECTS) -o $@


test: $(EXECUTABLE)
	$(EXECUTABLE)


clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f $(HIP_PATH)/src/*.o