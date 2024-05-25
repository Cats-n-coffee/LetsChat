#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

// =========== All connections
int listener;
std::vector<pollfd> allClients;


// =========== Server
int main()
{
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

    pollfd listenerSocket;
    listenerSocket.fd = tcpSocket;
    listenerSocket.events = POLLIN;

    allClients.push_back(listenerSocket);

    listener = tcpSocket;

    // ============== Main loop with poll
    while (true) {
        int numberOfEvents = poll(allClients.data(), (nfds_t) allClients.size(), 2000);
        std::cout << "poll num " << numberOfEvents << std::endl;
        if (numberOfEvents == -1) {
            std::cout << "poll error" << strerror(errno) << std::endl;
            return -1;
        }

        std::vector<int> disconnectedClients;

        for (int i = 0; i < allClients.size(); i++) {
            if (allClients[i].revents & POLLIN) {
                if (allClients[i].fd == listener) {
                    std::cout << "SERVER!!" << std::endl;
                    struct sockaddr_in connectionSocketAddress;
                    socklen_t connectionSocketAddressSize = sizeof(connectionSocketAddress);
                    int connectedSocket = accept(tcpSocket, (struct sockaddr *) &connectionSocketAddress, &connectionSocketAddressSize);
                    
                    std::cout << "after accept" << connectedSocket << std::endl;

                    if (connectedSocket == -1) {
                        std::cout << "error accept" << strerror(errno) << std::endl;
                        return -1;
                    }

                    pollfd connectedClient;
                    connectedClient.fd = connectedSocket;
                    connectedClient.events = POLLIN;

                    allClients.push_back(connectedClient);
                }
                else {
                    std::cout << "INCOMING!!" << std::endl;
                    // ============= Read message from client
                    char messageBuf[100];
                    ssize_t receivedMessage = recv(allClients[i].fd, messageBuf, sizeof(messageBuf), 0);

                    if (receivedMessage == -1) {
                        std::cout << "error recv " << strerror(errno) << std::endl;
                        return -1;
                    }

                    std::cout << "RECV return " << receivedMessage << std::endl;

                    if (receivedMessage > 0) {
                        std::cout << "RECEIVED: " << std::string(messageBuf) << std::endl;
                        // push message to other connected clients
                        // should we have a separate send function
                        for (int j = 0; j < allClients.size(); j++) {
                            if (allClients[j].fd != listener && allClients[j].fd != allClients[i].fd) {
                                if (send(allClients[j].fd, messageBuf, receivedMessage, 0) == -1) {
                                    std::cout << "error in send " << strerror(errno) << std::endl;
                                }
                            }
                        }
                    }
                    
                    if (receivedMessage == 0) { // Begin handling client disconnections
                        // Push client index as it currently is in the vector allClients
                        disconnectedClients.push_back(i);
                    }
                }
            }
        }

        // Remove disconnected clients from the vector
        for (int i = 0; i < disconnectedClients.size(); i++) {
            allClients.erase(allClients.begin() + disconnectedClients[i]);
        }
        disconnectedClients.clear();
        std::cout << "After all " << allClients.size() << std::endl;
    }

    // ============== Close socket
    if (close(tcpSocket) == -1) {
        std::cout << "error closing" << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Server closed" << std::endl;
    return 0;
}