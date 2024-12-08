// TcpClient.h
#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <netinet/in.h>

class TcpClient {
public:
    // Constructor: initializes the client with server IP and port number
    TcpClient(const std::string& serverIp, int port);

    // Connects the client to the server.
    // Returns true on success, false on failure.
    bool connectToServer();

    // Starts the communication with the server.
    void startCommunication();

    // Destructor: Closes the connection if open
    ~TcpClient();

private:
    // IP address of the server
    std::string serverIp;

    // Port number of the server
    int port;

    // Socket descriptor for the client
    int clientSd;

    // Address information of the server
    sockaddr_in sendSockAddr;

    // Handles the communication process with the server
    void handleCommunication();
};

#endif // TCP_CLIENT_H