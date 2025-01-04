AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/neural -I$(top_srcdir)/src/tcp_client
AM_CXXFLAGS = --offload-arch=gfx1036 --offload-arch=gfx1100 -g

EXTRA_DIST = config.yaml

# Custom rule to copy config.yaml
copy-config-file:
	@echo "Copying config.yaml to build directory..."
	@mkdir -p $(top_builddir)/src
	@cp $(srcdir)/config.yaml $(top_builddir)/src/config.yaml

# Ensure the copy-config-file rule runs before compiling
all-local: copy-config-file

lib_LIBRARIES = libneural_network.a
libneural_network_a_SOURCES = neural_network.cpp neural/BaseNeuron.cpp neural/SigmoidNeuron.cpp tcp_client/TcpClient.cpp

bin_PROGRAMS = neural_network
neural_network_SOURCES = neural_network.cpp \
                         neural/BaseNeuron.cpp \
                         neural/SigmoidNeuron.cpp \
                         tcp_client/TcpClient.cpp
neural_network_LDADD = -L/opt/rocm/lib -lrocrand -lyaml-cpp -limgui -lSDL3