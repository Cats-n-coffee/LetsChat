#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// =========== All connections
std::vector<int> allClients = {};

// =========== Threads handlers

// Create thread to loop over connected clients
void handleMessages(std::vector<int> connections)
{
    std::cout << "======= inside message thread" << std::endl;
    // use poll() here
    // for (int connectedSocket : connections) {
    //     std::cout << "======= current " << connectedSocket << std::endl;

    //     // ============= Read message from client
    //     char messageBuf[100];
    //     ssize_t receivedMessage = recv(connectedSocket, messageBuf, sizeof(messageBuf), 0);

    //     if (receivedMessage == -1) {
    //         std::cout << "error recv " << strerror(errno) << std::endl;
    //         // return -1;
    //     }

    //     std::cout << "received message length " << receivedMessage << " Message: " << messageBuf << std::endl;
    // }
    std::cout << "======= end of message thread" << std::endl;
}

// Accept client connections
void handleConnections(int tcpSocket)
{
    std::cout << "======= inside  accept thread" << std::endl;
    while (1) {
        struct sockaddr_in connectionSocketAddress;
        socklen_t connectionSocketAddressSize = sizeof(connectionSocketAddress);
        int connectedSocket = accept(tcpSocket, (struct sockaddr *) &connectionSocketAddress, &connectionSocketAddressSize);
        
        std::cout << "after accept" << connectedSocket << std::endl;

        if (connectedSocket == -1) {
            std::cout << "error accept" << strerror(errno) << std::endl;
            // return -1;
        }

        allClients.push_back(connectedSocket);
    }
    std::cout << "======= end of accept thread" << std::endl;
}

// =========== Server
int main()
{
    std::vector<std::thread> threads = {};

    // =========== Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "after creation" << std::endl;

    // ============ Bind socket to an address and port, so clients can find it
    struct sockaddr_in socketAddr;

    socketAddr.sin_family = AF_INET; // Address family
    socketAddr.sin_port = htons(5051); // Port
    inet_aton("127.0.0.1", &socketAddr.sin_addr); // ipv4 address


    if (bind(tcpSocket, (struct sockaddr*) &socketAddr, sizeof(socketAddr)) == -1) {
        std::cout << "error bind" << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "after bind" << std::endl;

    // ============== Listen for incoming connections
    if (listen(tcpSocket, 50) == -1) {
        std::cout << "error listen" << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "after listen" << std::endl;

    // ============  Create threads for connections and messages

    threads.push_back(std::thread(handleConnections, tcpSocket));
    threads.push_back(std::thread(handleMessages, allClients));

    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    // ============== Close socket
    if (close(tcpSocket) == -1) {
        std::cout << "error closing" << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Server closed" << std::endl;
    return 0;
}