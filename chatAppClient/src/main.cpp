#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
// #include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    // ============ Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << std::endl; // check error logging
        return -1;
    }
    std::cout << "after creation" << std::endl;

    // ============= Connect to server
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET; // Address family
    serverAddr.sin_port = htons(5051); // Port
    inet_aton("127.0.0.1", &serverAddr.sin_addr); // ipv4 address

    if (connect(tcpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        // handle error
        std::cout << "error connection" << std::endl;
        return -1;
    }

    std::cout << "after connection" << std::endl;

    // ============= Send message
    std::string message = "This is a message"; // should prob check the length
    char buf[100];
    strncpy(buf, message.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = 0;

    if (send(tcpSocket, buf, sizeof(buf), 0) == -1) {
        // handle error
        std::cout << "error send" << std::endl;
        return -1;
    }
    std::cout << "after send" << std::endl;

    // accept requests?
    // struct sockaddr_in connectionSocketAddress;
    // socklen_t connectionSocketAddressSize = sizeof(connectionSocketAddress);
    // int connectionSocket = accept(tcpSocket, (struct sockaddr *) &connectionSocketAddress, &connectionSocketAddressSize);
    // std::cout << "after accept" << std::endl;
    // if (connectionSocket == -1) {
    //     // handle error
    //     std::cout << "error accept" << std::endl;
    //     return -1;
    // }
    // read?

    // send?

}