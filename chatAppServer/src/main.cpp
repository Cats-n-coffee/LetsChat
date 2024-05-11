#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    // =========== Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << std::endl; // TODO check error logging
        return -1;
    }
    std::cout << "after creation" << std::endl;

    // ============ Bind socket to an address and port, so clients can find it
    struct sockaddr_in socketAddr;

    socketAddr.sin_family = AF_INET; // Address family
    socketAddr.sin_port = htons(5051); // Port
    inet_aton("127.0.0.1", &socketAddr.sin_addr); // ipv4 address


    if (bind(tcpSocket, (struct sockaddr*) &socketAddr, sizeof(socketAddr)) == -1) {
        // TODO handle error
        std::cout << "error bind" << std::endl;
        return -1;
    }
    std::cout << "after bind" << std::endl;

    // ============== Listen for incoming connections
    if (listen(tcpSocket, 50) == -1) {
        // TODO handle error
        std::cout << "error listen" << std::endl;
        return -1;
    }
    std::cout << "after listen" << std::endl;

    // ============= Accept client connections
    while (1) {
        struct sockaddr_in connectionSocketAddress;
        socklen_t connectionSocketAddressSize = sizeof(connectionSocketAddress);
        int connectedSocket = accept(tcpSocket, (struct sockaddr *) &connectionSocketAddress, &connectionSocketAddressSize);
        
        std::cout << "after accept" << connectedSocket << std::endl;

        if (connectedSocket == -1) {
            // TODO handle error
            std::cout << "error accept" << std::endl;
            return -1;
        }

        // ============= Read message from client
        char messageBuf[100];
        ssize_t receivedMessage = recv(connectedSocket, messageBuf, sizeof(messageBuf), 0);

        if (receivedMessage == -1) {
            // TODO handle error
            std::cout << "error recv" << std::endl;
            return -1;
        }

        std::cout << "received message length " << receivedMessage << " Message: " << messageBuf << std::endl;
    }

    // ============== Close socket
    if (close(tcpSocket) == -1) {
        // handle error
        std::cout << "error closing" << std::endl;
        return -1;
    }
}