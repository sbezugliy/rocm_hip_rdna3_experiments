Add here buffer and send data from buffer synchronously to the server, If I append data from main program by pointer
For datagrams I will use cbor objects
Start conversation should be with 
```
M: S
``` 
and wait for answear with `
```
M: S
S: <UUID of session>`
```
After should send everything coming to buffer as 
```
M: D
S: <UUID of session>
C: int serial number of records in buffer
P: any data type payload
``` 
And wait for after each :
```
M: D
R: O or E // OK or ERROR
S: <UUID of session>` after each record from server it should send next record
C: number of messages in a session
P: everything serializable by cbor
```
On `ERR:  <any error text with any chars>` it should raise error and drop session
```
M: X
S: <UUID of session>
R: E
P:  repeat error reason
```
, the same if C is not in a row, drop session with 
but `P` should be lost `Lost message order`

Add configurable timeout of 60 seconds between each 
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

To implement the requirements for adding a buffer and sending data synchronously to the server using the specified protocol, you need to modify the `TcpClient` class to manage a buffer and handle the session control flow as described. Here's how you can do it:

```cpp
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <vector>
#include <stdexcept>
#include "cbor.h" // Assuming you have a CBOR library for C++

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

    std::vector<std::vector<uint8_t>> buffer;
    int serialNumber;

    void handleCommunication();
    void sendData(const std::vector<uint8_t>& data);
    void receiveData();
    void initiateSession();
    void handleServerResponse(const std::string& response);
};

TcpClient::TcpClient(const std::string& serverIp, int port)
    : serverIp(serverIp), port(port), clientSd(-1), serialNumber(0) {
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

void TcpClient::initiateSession() {
    std::string request = "M: S";
    send(clientSd, request.c_str(), request.size(), 0);

    char response[1500];
    recv(clientSd, response, sizeof(response), 0);
    std::string sessionResponse(response);

    handleServerResponse(sessionResponse);
}

void TcpClient::handleServerResponse(const std::string& response) {
    if (response.substr(0, 3) == "S: ") {
        std::string sessionId = response.substr(3);
        std::cout << "Session established with ID: " << sessionId << std::endl;

        while (!buffer.empty()) {
            std::vector<uint8_t> data = buffer.front();
            buffer.erase(buffer.begin());

            sendData(data); // Function to handle data sending
            receiveData();  // Await server response
        }
    } else {
        throw std::runtime_error("Failed to establish session: " + response);
    }
}

void TcpClient::handleCommunication() {
    initiateSession();
    // Additional handling logic...
}

void TcpClient::sendData(const std::vector<uint8_t>& data) {
    std::string header = "M: D\nS: " + std::to_string(serialNumber) + "\nC: " + std::to_string(serialNumber) + "\nP: ";
    std::vector<uint8_t> cborData = cbor_serialize(data); // Assuming you have a CBOR serialization function
    send(clientSd, header.data(), header.size(), 0);
    send(clientSd, cborData.data(), cborData.size(), 0);
}

void TcpClient::receiveData() {
    char response[1500];
    recv(clientSd, response, sizeof(response), 0);
    std::string serverResponse(response);

    if (serverResponse.find("ERR:") != std::string::npos) {
        std::cerr << "Error received from server: " << serverResponse << std::endl;
        throw std::runtime_error("Session error");
    } else {
        std::cout << "Server response: " << serverResponse << std::endl;

        // Handle OK or ERROR in server response
        if (serverResponse.find("R: O") != std::string::npos) {
            serialNumber++;
        } else if (serverResponse.find("R: E") != std::string::npos) {
            throw std::runtime_error("Error during transmission");
        }
    }
}

void TcpClient::startCommunication() {
    struct timeval start, end;
    gettimeofday(&start, nullptr);

    try {
        handleCommunication();
    } catch (const std::exception& e) {
        std::cerr << "Communication error: " << e.what() << std::endl;
    }

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
```

### Key Points:
1. **Buffer Management**: Implement a buffer as a vector of vectors to store serialized CBOR data.
2. **Session Handling**: Implement a method to initiate sessions and manage server responses.
3. **Data Transmission**: Implement `sendData` and `receiveData` methods to send buffered data according to the protocol.
4. **Error Handling**: Incorporate error conditions as described, raising exceptions and terminating sessions as necessary.
5. **CBOR Serialization**: Use a CBOR library for data serialization (the implementation assumes a suitable library is included).

**Note**: You'll need to include a CBOR library and modify the code accordingly to ensure it compiles and works with CBOR data. This would include implementing or using existing functions for CBOR serialization and deserialization, such as `cbor_serialize` used in the code.

Certainly! To better manage the communication protocol and its states, we can implement a state machine that handles the different states and transitions based on events. This approach can help to organize the code more clearly and make it easier to maintain.

Let's start by defining a basic state machine structure. We'll create a `SessionStateMachine` class that encapsulates the logic for handling the protocol's different states and events. I'll also include a simple event enumeration and some sample code for integrating this into the existing `TcpClient` class.

### State Machine Implementation

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include "cbor.h"

class SessionStateMachine {
public:
    enum class State {
        Idle,
        SessionStarted,
        SendingData,
        SessionEnded,
    };

    enum class Event {
        StartSession,
        SendData,
        ReceiveAck,
        ReceiveError,
        EndSession,
    };

    SessionStateMachine();
    void handleEvent(Event event, const std::string& payload = "");
    void setSessionId(const std::string& sessionId);
  
private:
    State currentState;
    std::string sessionId;
    std::vector<std::vector<uint8_t>> buffer;
    int serialNumber;
    void transition(Event event, const std::string& payload);
    void onDataSent();
    void onAckReceived();
    void onErrorReceived(const std::string& error);
};

SessionStateMachine::SessionStateMachine()
    : currentState(State::Idle), serialNumber(0) {}

void SessionStateMachine::handleEvent(Event event, const std::string& payload) {
    try {
        transition(event, payload);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Reset to idle in case of error
        currentState = State::Idle;
    }
}

void SessionStateMachine::setSessionId(const std::string& sessionId) {
    this->sessionId = sessionId;
}

void SessionStateMachine::transition(Event event, const std::string& payload) {
    switch (currentState) {
        case State::Idle:
            if (event == Event::StartSession) {
                currentState = State::SessionStarted;
                std::cout << "Session started with ID: " << sessionId << std::endl;
            }
            break;

        case State::SessionStarted:
            if (event == Event::SendData && !buffer.empty()) {
                currentState = State::SendingData;
                // Insert logic to send data here
                onDataSent();
            }
            break;

        case State::SendingData:
            if (event == Event::ReceiveAck) {
                currentState = State::SessionStarted; // Ready to send next data
                onAckReceived();
            } else if (event == Event::ReceiveError) {
                onErrorReceived(payload);
            }
            break;

        case State::SessionEnded:
            //final clean-up if necessary
            std::cout << "Session ended." << std::endl;
            break;
        
        default:
            throw std::runtime_error("Unknown state");
    }
}

void SessionStateMachine::onDataSent() {
    // Simulate sending data logic; in reality, you should integrate actual send logic
    std::cout << "Data sent with serial number: " << serialNumber << std::endl;
    serialNumber++;
}

void SessionStateMachine::onAckReceived() {
    std::cout << "Acknowledgment received for serial number: " << serialNumber-1 << std::endl;
}

void SessionStateMachine::onErrorReceived(const std::string& error) {
    std::cerr << "Error received: " << error << std::endl;
    currentState = State::SessionEnded;
}
```

### Integration with TcpClient

You will modify the `TcpClient` class to use this `SessionStateMachine` to handle the logic:

```cpp
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
    SessionStateMachine stateMachine;

    void initiateSession();
    void sendData();
    void receiveData();
    void onServerResponse(const std::string& response);
};

void TcpClient::initiateSession() {
    stateMachine.handleEvent(SessionStateMachine::Event::StartSession);

    std::string request = "M: S";
    send(clientSd, request.c_str(), request.size(), 0);

    char response[1500];
    recv(clientSd, response, sizeof(response), 0);
    std::string sessionResponse(response);
    onServerResponse(sessionResponse);
}

void TcpClient::onServerResponse(const std::string& response) {
    if (response.substr(0, 3) == "S: ") {
        stateMachine.setSessionId(response.substr(3));
        stateMachine.handleEvent(SessionStateMachine::Event::SendData);
    } else if (response.find("ERR: ") != std::string::npos) {
        stateMachine.handleEvent(SessionStateMachine::Event::ReceiveError, response);
    }
}

void TcpClient::sendData() {
    // Logic for sending data and calling the state machine's event
    stateMachine.handleEvent(SessionStateMachine::Event::SendData);
    receiveData(); // Await response
}

void TcpClient::receiveData() {
    char response[1500];
    recv(clientSd, response, sizeof(response), 0);
    std::string serverResponse(response);

    if (serverResponse.find("R: O") != std::string::npos) {
        stateMachine.handleEvent(SessionStateMachine::Event::ReceiveAck);
    } else if (serverResponse.find("R: E") != std::string::npos) {
        stateMachine.handleEvent(SessionStateMachine::Event::ReceiveError, serverResponse);
    }
}
```

### Key Modifications and Design

1. **State Machine**: We created a `SessionStateMachine` class that clearly defines the states and state transitions caused by events.
2. **Event Handling**: The `TcpClient` class is refactored to use the state machine for handling different events such as starting a session, sending data, receiving acknowledgment, or detecting errors.
3. **Loose Coupling**: The communication logic and state control logic are separated, which allows better maintainability and understanding of the code.

With this setup, you can focus on the state transitions and events independently from the specific network operations, making it clearer how protocol states and interactions are handled.

Using a state machine to manage protocol states and transitions is generally a good approach, especially for protocols with clearly defined states and flows like the one you described. Here are some reasons why it can be beneficial:

1. **Clear Structure**: State machines provide a clear and structured way to describe the behavior of a system, making the flow of states and transitions explicit and easier to understand.

2. **Maintainability**: By separating the state management logic from the rest of the application, you can more easily modify and maintain the code. Changes to state transitions or handling can be made in one place, reducing the risk of introducing errors.

3. **Scalability**: As the protocol or system grows in complexity, a state machine helps manage added complexity by organizing different states and transitions clearly. It can handle more intricate logic without becoming unwieldy.

4. **Reliability**: State machines naturally follow a deterministic sequence of states, making it easier to reason about possible states of the system and reducing the likelihood of unexpected behavior.

5. **Testability**: States and transitions can be tested independently, making it easier to isolate and verify the correctness of specific parts of the protocol handling.

However, there are some considerations to keep in mind:

1. **Overhead**: For very simple protocols, a state machine might add unnecessary complexity. However, in your case, the protocol involves multiple state transitions and error handling, which justifies a state machine.

2. **Learning Curve**: Developers unfamiliar with state machines might find them somewhat abstract or confusing at first. Clear documentation and examples can help with this.

3. **Code Duplication**: Without careful design, there can be code duplication, especially if similar logic needs to be executed in multiple states.

Overall, if the protocol's logic fits well into a state machine paradigm, it can greatly benefit organization, clarity, and maintainability. Given the requirements you described, with specific states and transitions (session initiation, data sending, error handling, etc.), a state machine approach is a good fit. Just ensure to document the states and transitions clearly to maximize its benefits.

Here are some point of common design I need synchronous messaging chain inside of context of same session
But I want to be able keep and process sessions asynchronously

To design a system that allows synchronous messaging within the context of the same session, while managing multiple sessions asynchronously, you can follow a few key principles. This setup is particularly useful in scenarios where each session follows a defined sequence of operations (like a protocol) but multiple sessions can be handled concurrently.

### Overall Design

1. **Session Management**: Each session should be encapsulated within a context (e.g., a class or a data structure) that maintains its state, data, and any necessary protocol handling logic.

2. **Concurrency Handling**: Use asynchronous processing to manage multiple session contexts concurrently. This could involve threads, coroutines, or asynchronous I/O depending on your language and environment.

3. **Synchronous Messaging Inside Sessions**: Within a session context, use synchronous calls to ensure the ordered, step-by-step execution of protocol states and message handling.

4. **State Machine for Each Session**: Implement a state machine within each session context to manage states and transitions, ensuring that each session operates within its defined protocol.

### Implementation Strategy

Here’s how you can achieve this in C++ using modern C++ techniques with threading:

#### Session Context and State Machine

1. **Session Class**: Define a `Session` class that holds the state of a session, including its identifier, data buffer, state machine, and any session-specific logic.

2. **State Machine and Event Handling**: Each `Session` class manages its protocol through a state machine, handling events and states synchronously within the session context.

3. **Asynchronous Management with Threads or Asynchronous I/O**: Use threads or asynchronous I/O to manage multiple sessions concurrently. Each session runs its logic synchronously, but sessions themselves are processed asynchronously.

#### Example Code

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

// Mock of CBOR serialize function
std::vector<uint8_t> cbor_serialize(const std::vector<uint8_t>& data) {
    return data; // Simplification for illustration
}

// Event-driven state machine for a session
class SessionStateMachine {
public:
    // Define possible states and events for a session
    enum class State { Idle, SessionStarted, SendingData, SessionEnded };
    enum class Event { StartSession, SendData, ReceiveAck, ReceiveError, EndSession };

    SessionStateMachine() : currentState(State::Idle), serialNumber(0) {}

    void handleEvent(Event event, const std::string& payload = "") {
        // Handle transitions and actions based on the state/event
        transition(event, payload);
    }

private:
    State currentState;
    int serialNumber;

    void transition(Event event, const std::string& payload) {
        switch (currentState) {
            case State::Idle:
                if (event == Event::StartSession) {
                    // Transition logic here
                    currentState = State::SessionStarted;
                    std::cout << "Session started." << std::endl;
                }
                break;
            case State::SessionStarted:
                if (event == Event::SendData) {
                    currentState = State::SendingData;
                    // Logic to send data
                    onDataSent();
                }
                break;
            // Additional state transitions...
            case State::SessionEnded:
                std::cout << "Session ended." << std::endl;
                break;
            default:
                break;
        }
    }

    void onDataSent() {
        std::cout << "Data sent, serial: " << ++serialNumber << std::endl;
    }
};

// Class to encapsulate a session's logic
class Session {
public:
    Session(int id) : sessionId(id), stateMachine() {}

    void process() {
        // Simulate the session processing logic
        stateMachine.handleEvent(SessionStateMachine::Event::StartSession);
        stateMachine.handleEvent(SessionStateMachine::Event::SendData);
        stateMachine.handleEvent(SessionStateMachine::Event::ReceiveAck);
        stateMachine.handleEvent(SessionStateMachine::Event::EndSession);
    }

private:
    int sessionId;
    SessionStateMachine stateMachine;
};

// Multithreaded session manager
class SessionManager {
public:
    void addSession(int sessionId) {
        std::lock_guard<std::mutex> lock(mutex);
        sessions.emplace(sessionId, std::make_unique<Session>(sessionId));
        condition.notify_one();
    }

    void run() {
        while (true) {
            std::unique_ptr<Session> session;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [&]() { return !sessions.empty(); });
                auto it = sessions.begin();
                session = std::move(it->second);
                sessions.erase(it);
            }
            session->process(); // Process session synchronously within its own context
        }
    }

private:
    std::mutex mutex;
    std::condition_variable condition;
    std::unordered_map<int, std::unique_ptr<Session>> sessions;
};

int main() {
    SessionManager manager;
    std::thread managerThread(&SessionManager::run, &manager);

    // Simulate adding sessions from different threads
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        manager.addSession(i);
    }

    managerThread.join();
    return 0;
}
```

### Key Design Considerations

- **Concurrency Handling**: Each `Session` processes its actions synchronously, but the session manager dispatches and manages sessions asynchronously across threads.
- **State Management**: Each `Session` uses a `SessionStateMachine` to handle protocol logic, ensuring that each acts independently and synchronously.
- **Scalability**: Such a design scales well with the number of sessions, as each session can be processed independently on different threads.

This design effectively isolates session state management and allows you to flexibly handle multiple sessions concurrently, maintaining ordered processing within each session while keeping the overall system responsive and scalable.