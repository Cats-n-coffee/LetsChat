#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int clientSocket;

void sendMessageHandler()
{
    std::string userMessage = "";

    while (true) {
        std::cout << "Enter your message: " << std::endl;
        std::getline(std::cin, userMessage);

        char buf[100];
        strncpy(buf, userMessage.c_str(), sizeof(buf));
        buf[sizeof(buf) - 1] = 0;

        if (send(clientSocket, buf, sizeof(buf), 0) == -1) {
            std::cout << "Error sending message, try reconnecting " << strerror(errno) << std::endl;
            exit(1);
        }

        userMessage = "";
    }
}

void receiveMessageHandler()
{
    while (true) {
        char incomingMessage[100];
        ssize_t messageLength = recv(clientSocket, incomingMessage, sizeof(incomingMessage), 0);
        
        if (messageLength == -1) {
            std::cout << "Error receiving from server, try reconnecting " << strerror(errno) << std::endl;
            exit(1);
        }

        std::cout << "--------------- FROM Server: " << std::string(incomingMessage) << std::endl;
    }
}

int main()
{
    std::vector<std::thread> clientThreads;

    // ============ Create a socket
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (tcpSocket == -1) {
        std::cout << "error creation" << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "after creation" << std::endl;
    clientSocket = tcpSocket;

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

    // ============= Send/Receive messages

    clientThreads.push_back(std::thread(sendMessageHandler));
    clientThreads.push_back(std::thread(receiveMessageHandler));

    for (int i = 0; i < clientThreads.size(); i++) {
        clientThreads[i].join();
    }    

    // ============== Close Socket
    if (close(tcpSocket) == -1) {
        std::cout << "error closing" << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Client closed" << std::endl;
    return 0;
}