// TcpClient.cpp
#include "TcpClient.hpp"
#include <iostream>
#include <cstring> // for memset
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <cerrno>

TcpClient::TcpClient(const std::string& serverIp, int port)
    : serverIp(serverIp), port(port), clientSd(-1) {
    memset(&sendSockAddr, 0, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &sendSockAddr.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address or address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool TcpClient::connectToServer() {
    clientSd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSd < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return false;
    }

    int status = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if (status < 0) {
        std::cerr << "Error connecting to socket: " << strerror(errno) << std::endl;
        close(clientSd);
        return false;
    }

    std::cout << "Connected to the TCP server on `" << serverIp << ":" << port << "` !" << std::endl;
    return true;
}

void TcpClient::handleCommunication() {
    char msg[1500];
    ssize_t bytesRead = 0, bytesWritten = 0;

    while (true) {
        std::cout << ">";
        std::string data;
        std::getline(std::cin, data);
        memset(msg, 0, sizeof(msg)); // Clear the buffer
        strncpy(msg, data.c_str(), sizeof(msg) - 1);
        
        if (data == "exit") {
            send(clientSd, msg, strlen(msg), 0);
            break;
        }
        
        bytesWritten = send(clientSd, msg, strlen(msg), 0);
        std::cout << "Awaiting server response..." << std::endl;
        memset(msg, 0, sizeof(msg)); // Clear the buffer
        bytesRead = recv(clientSd, msg, sizeof(msg) - 1, 0);
        if (bytesRead <= 0) {
            std::cerr << "Error reading from server or server closed the connection." << std::endl;
            break;
        }
        msg[bytesRead] = '\0'; // Null-terminate the string

        if (strcmp(msg, "exit") == 0) {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << "Server: " << msg << std::endl;
    }

    std::cout << "********Session********" << std::endl;
    std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << std::endl;
}

void TcpClient::startCommunication() {
    struct timeval start, end;
    gettimeofday(&start, nullptr);

    handleCommunication();

    gettimeofday(&end, nullptr);
    close(clientSd);
    std::cout << "Elapsed time: " << (end.tv_sec - start.tv_sec) << " secs" << std::endl;
    std::cout << "Connection closed" << std::endl;
}

TcpClient::~TcpClient() {
    if (clientSd >= 0) {
        close(clientSd);
    }
}