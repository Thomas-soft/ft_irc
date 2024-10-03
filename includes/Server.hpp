#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <cstdio>
#include <sstream>
#include "Client.hpp"
#include "Commands.hpp"

class Client;

class Server
{
    private:
        int _port;
        int _server_fd;
        struct sockaddr_in  _address;
        std::string _pass;
        std::vector<pollfd> _pollfd;
        std::vector<Client> _client;
        Server();

    public:
        Server(int port, std::string pass);
        ~Server();
        void    start();
        void    accept_client();
        void    read_client(size_t i);
        void    parse(char *buffer, size_t i);
        int		get_client_fd(int fd);
};
