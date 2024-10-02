#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstring> 

int main(int argc, char **argv)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    const char * msg = argv[1];

    sockaddr_in clientInfo;

    clientInfo.sin_family = AF_INET;
    clientInfo.sin_port = htons(45181);
    clientInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(client_socket, (struct sockaddr*)&clientInfo, sizeof(clientInfo));
    std::cout << "Connected to the server!" << std::endl;
    send(client_socket, msg, strlen(msg), 0);
    close(client_socket);
}