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

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1)
        ft_exit("Server socket");
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
        ft_exit("Server setsockopt");
    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
        ft_exit("Server bind");
    if (listen(_server_fd, 5) == -1)
        ft_exit("Server listen");
    pollfd  server_pollfd = {_server_fd, POLLIN, 0};
    _pollfd.push_back(server_pollfd);
    std::cout << "Server started on port " << _port << std::endl;
    while (1)
    {
        int status = 0;

        status = poll(_pollfd.data(), _pollfd.size(), -1);
        if (status == -1)
            ft_exit("Server poll");
        if (status == 0)
            continue ;
        for (size_t i = 0; i < _pollfd.size(); i++)
        {
            if (_pollfd[i].revents & POLLIN)
            {
                if (_pollfd[i].fd == _server_fd)
                    accept_client();
                else
                    read_client(i);
            }
        }
    }
}

void    Server::accept_client()
{
    int client_socket;
    struct sockaddr_in  client_address;
    socklen_t sizeof_client_address = sizeof(client_address);

    client_socket = accept(_server_fd, (struct sockaddr *)&client_address, &sizeof_client_address);
    if (client_socket == -1)
        perror("Server accept");
    pollfd  client_pollfd = {client_socket, POLLIN, 0};
    Client  client(client_pollfd.fd);
    _pollfd.push_back(client_pollfd);
	_client.push_back(client);
    std::cout << "New client connected!" << std::endl;
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
        parse(buffer);
    }
}

void    Server::parse(char *buffer)
{
    std::string cmd = "";
    std::vector<std::string>    args_list;
    std::stringstream   stream(buffer);

    stream >> cmd;
    std::string word = "";
    std::stringstream   args_stream(stream.str().substr(cmd.size()));
    while (args_stream >> word)
        args_list.push_back(word);
    // display args
    for (size_t i = 0; i < args_list.size(); i++)
        std::cout << "args[" << i << "] = " << args_list[i] << std::endl;
    // execute_cmd(cmd, args, *this);
}

void    Server::close_all_fd()
{
    for (size_t i = 0; i < _pollfd.size(); i++)
    {
        close(_pollfd[i].fd);
    }
    close(_server_fd);
}

void    Server::ft_exit(std::string error)
{
    perror(error.c_str());
    close_all_fd();
    exit(EXIT_FAILURE);
}
