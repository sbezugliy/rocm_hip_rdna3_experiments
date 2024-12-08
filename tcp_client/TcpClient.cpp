//Client side
#include <iostream>
#include <string>
#include <cstring> // for bzero and memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

class TcpClient {
public:
    TcpClient(const std::string& serverIp, int port);
    bool connectToServer();
    void startCommunication();
    ~TcpClient();

private:
    std::string serverIp;
    int port;
    int clientSd;
    sockaddr_in sendSockAddr;

    void handleCommunication();
};

TcpClient::TcpClient(const std::string& serverIp, int port)
    : serverIp(serverIp), port(port), clientSd(-1) {
    memset(&sendSockAddr, 0, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_port = htons(port);

    struct hostent* host = gethostbyname(serverIp.c_str());
    if (host == nullptr) {
        std::cerr << "Error obtaining host information" << std::endl;
        exit(EXIT_FAILURE);
    }
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
}

bool TcpClient::connectToServer() {
    clientSd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    int status = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if (status < 0) {
        std::cerr << "Error connecting to socket!" << std::endl;
        return false;
    }

    std::cout << "Connected to the TCP server on `" << serverIp << ":" << port << "` !" << std::endl;
    return true;
}

void TcpClient::handleCommunication() {
    char msg[1500];
    int bytesRead = 0, bytesWritten = 0;

    while (true) {
        std::cout << ">";
        std::string data;
        std::getline(std::cin, data);
        memset(msg, 0, sizeof(msg)); // Clear the buffer
        strcpy(msg, data.c_str());
        if (data == "exit") {
            send(clientSd, msg, strlen(msg), 0);
            break;
        }
        bytesWritten += send(clientSd, msg, strlen(msg), 0);
        std::cout << "Awaiting server response..." << std::endl;
        memset(msg, 0, sizeof(msg)); // Clear the buffer
        bytesRead += recv(clientSd, msg, sizeof(msg), 0);
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