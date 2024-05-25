#include <iostream>
#include <string>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    // ============ Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "after creation" << std::endl;

    // ============= Connect to server
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET; // Address family
    serverAddr.sin_port = htons(5051); //  Server Port
    inet_aton("127.0.0.1", &serverAddr.sin_addr); // ipv4 address

    int connection = connect(tcpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (connection == -1) {
        std::cout << "error connection " << strerror(errno) << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "after connection" << std::endl;

    // ============= Send message
    while (true) {
        // Probably need to check for connection to still be there?

        // Write
        std::string userMessage = "";

        std::cout << "Enter your message: " << std::endl;
        std::getline(std::cin, userMessage);
        std::cout << "entrered " << userMessage << std::endl;

        char buf[100];
        strncpy(buf, userMessage.c_str(), sizeof(buf));
        buf[sizeof(buf) - 1] = 0;

        if (send(tcpSocket, buf, sizeof(buf), 0) == -1) {
            std::cout << "error send" << strerror(errno) << std::endl;
            return -1;
        }
        std::cout << "after send" << std::endl;

        // Read
        char incomingMessage[100];
        ssize_t messageLength = recv(tcpSocket, incomingMessage, sizeof(incomingMessage), 0);
        
        if (messageLength == -1) {
            std::cout << "error receiving from server " << strerror(errno) << std::endl;
        }

        std::cout << "FROM Server: " << std::string(incomingMessage) << std::endl;
    }
    

    // ============== Close Socket
    if (close(tcpSocket) == -1) {
        std::cout << "error closing" << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Client closed" << std::endl;
    return 0;
}