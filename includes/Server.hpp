#pragma once



#include <iostream>
#include <vector>
#include <cstdlib>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>


class Server
{
    private:
        int _port;
        int _server_fd;
        struct sockaddr_in  _address;
        std::string _pass;
        std::vector<pollfd> _pollfd;
        Server();

    public:
        Server(int port, std::string pass);
        ~Server();
        void    start();
        void    accept_client();
        void    read_client();
};
