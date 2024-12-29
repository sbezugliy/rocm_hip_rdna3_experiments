AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/src/neural -I$(top_srcdir)/src/tcp_client
AM_CXXFLAGS = --offload-arch=gfx1036 --offload-arch=gfx1100 -g

bin_PROGRAMS = neural_network
neural_network_SOURCES = neural_network.cpp \
                        hip_session.cpp \
                        neural/BaseNeuron.cpp \
                        neural/Neuron.cpp \
                        tcp_client/TcpClient.cpp
neural_network_LDADD = -L/opt/rocm/lib -lrocrand -lyaml-cpp -limgui -lSDL3