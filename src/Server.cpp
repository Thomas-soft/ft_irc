#include "../includes/Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass)
{
}

Server::~Server()
{
}

void    Server::start()
{
    int opt = 1;
    pollfd  server_pollfd;

    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1)
    {
        perror("Server socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = _port;
    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
    {
        perror("Server bind");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(_server_fd, 5) == -1)
    {
        perror("Server listen");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }

    server_pollfd.fd = _server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    _pollfd.push_back(server_pollfd);

    while (1)
    {
        int status = 0;

        status = poll(_pollfd.data(), _pollfd.size(), -1);
        if (status == -1)
        {
            perror("Pollfd");
            close(_server_fd);
            exit(EXIT_FAILURE);
        }
        if (status == 0)
            continue ;
        for (size_t i = 0; i < _pollfd.size(); i++)
        {
            if (_pollfd[i].revents & POLLIN)
            {
                // Information recu

                // Nouvelle connexion
                if (_pollfd[i].fd == _server_fd)
                {
                    accept_client();
                }
                // Client a envoye une info
                else
                {

                    read_client();
                }
            }
        }
    }
}

void    Server::accept_client()
{
    int new_socket;

    struct sockaddr_in  client_address;
    socklen_t client_len = sizeof(client_address);
    if ((new_socket = accept(_server_fd, (struct sockaddr *)&client_address, &client_len)) < 0)
        perror("accept");
    pollfd client_pollfd = {new_socket, POLLIN, 0};
    _pollfd.push_back(client_pollfd);
}

void    Server::read_client()
{
    
}