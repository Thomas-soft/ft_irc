// server.h
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

class Server {
public:
    Server();
    ~Server();
    void start(int port);
    void handleMessage(int client_fd, const char* message);

private:
    int server_fd;
};

#endif // SERVER_H