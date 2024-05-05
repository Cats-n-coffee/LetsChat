#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
// #include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    // Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << std::endl; // check error logging
        return -1;
    }
    std::cout << "after creation" << std::endl;
    
    // bind to an address and port?
    struct sockaddr_in socketAddr;

    socketAddr.sin_family = AF_INET; // Address family
    socketAddr.sin_port = htons(5051); // Port
    inet_aton("127.0.0.1", &socketAddr.sin_addr); // ipv4 address


    if (bind(tcpSocket, (struct sockaddr*) &socketAddr, sizeof(socketAddr)) == -1) {
        // handle error
        std::cout << "error bind" << std::endl;
        return -1;
    }
    std::cout << "after bind" << std::endl;
    // listen for requests?
    if (listen(tcpSocket, 50) == -1) {
        // handle error
        std::cout << "error listen" << std::endl;
        return -1;
    }
    std::cout << "after listen" << std::endl;

    // accept requests?
    struct sockaddr_in connectionSocketAddress;
    socklen_t connectionSocketAddressSize = sizeof(connectionSocketAddress);
    int connectedSocket = accept(tcpSocket, (struct sockaddr *) &connectionSocketAddress, &connectionSocketAddressSize);
    
    std::cout << "after accept" << std::endl;

    if (connectedSocket == -1) {
        // handle error
        std::cout << "error accept" << std::endl;
        return -1;
    }

    // read?
    char messageBuf[100];
    ssize_t receivedMessage = recv(connectedSocket, messageBuf, sizeof(messageBuf), 0);

    if (receivedMessage == -1) {
        // handle error
        std::cout << "error recv" << std::endl;
        return -1;
    }
    // send?
    std::cout << "received message length " << receivedMessage << " Message: " << messageBuf << std::endl;
}