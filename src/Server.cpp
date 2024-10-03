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
    _address.sin_port = htons(_port);
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
    std::cout << "Launched!" << std::endl;

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
                    read_client(i);
                }
            }
        }
    }
}

void    Server::accept_client()
{
    int new_socket;
    std::string nick;

    struct sockaddr_in  client_address;
    socklen_t client_len = sizeof(client_address);
    if ((new_socket = accept(_server_fd, (struct sockaddr *)&client_address, &client_len)) < 0)
        perror("accept");
    pollfd client_pollfd = {new_socket, POLLIN, 0};
    _pollfd.push_back(client_pollfd);
    Client new_client(client_pollfd.fd);
	_client.push_back(new_client);

}

int Server::get_client_fd(int fd)
{
	int i = 0;

	while (i < (int)_client.size())
	{
		if (_client[i].get_fd() == fd)
			return (i);
		i++;
	}
	return (-1);
}

void    Server::read_client(size_t i)
{
    char    buffer[512] = {0};

    if (recv(_pollfd[i].fd, buffer, sizeof(buffer), 0) <= 0)
    {
		_client.erase(_client.begin() + get_client_fd(_pollfd[i].fd));
        close(_pollfd[i].fd);
        _pollfd.erase(_pollfd.begin() + i);
        std::cout << "Client disconnected!" << std::endl;
    }
    else
    {
        std::cout << "Message from client (fd=" << _pollfd[i].fd << "): " << buffer << std::endl;
        parse(buffer, i);
    }
}

void    Server::parse(char *buffer, size_t i)
{
    // std::string cmd = buffer;
    std::string cmd = "";
    std::string args = "";
    std::stringstream   stream(buffer);
    (void)i;

    stream >> cmd;
    while (!std::getline(stream, args));
    //std::cout << cmd << args << std::endl;
    //execute_cmd(cmd, args, i);
}
