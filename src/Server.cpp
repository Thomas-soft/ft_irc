#include "../includes/Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass)
{
}

Server::~Server()
{
}


// --------------------- //
// Server start and loop //
// --------------------- //

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

void    Server::read_client(size_t i)
{
    char    buffer[512] = {0};

    if (recv(_pollfd[i].fd, buffer, sizeof(buffer), 0) <= 0)
    {
		_client.erase(_client.begin() + get_client_index(_pollfd[i].fd));
        close(_pollfd[i].fd);
        _pollfd.erase(_pollfd.begin() + i);
        std::cout << "Client disconnected!" << std::endl;
    }
    else
    {
        std::cout << "Message from client (fd=" << _pollfd[i].fd << "): " << buffer << std::endl;
        parse(buffer, this->get_client(_pollfd[i].fd));
    }
}


// --------------------- //
// Server parsing        //
// --------------------- //

void    Server::parse(char *buffer, Client &client)
{
    // execute_cmd(cmd, args, *this, client);
    (void)buffer;
    (void)client;

    std::string         line = buffer;
    std::vector<std::string>    lines;
    size_t  start = 0;
    size_t  end = 0;

    while (line.find("\r\n", start) != std::string::npos)
    {
        end = line.find("\r\n", start);
        if (end - start != 0)
            lines.push_back(line.substr(start, end - start));
        start = end + 2;
    }
    for (size_t i = 0; i < lines.size(); i++)
    {
        lines[i] = trim(lines[i]);
        std::string cmd = "";
        std::vector<std::string>    args;
        std::stringstream   stream(lines[i]);
        stream >> cmd;
        if (cmd == "")
            continue ;
        std::string word = "";
        std::stringstream   args_stream(stream.str().substr(cmd.size()));
        while (args_stream >> word)
            args.push_back(word);

        // DISPLAY

        std::cout << "cmd: " << "|" << cmd << "|" << std::endl;
        for (size_t i = 0; i < args.size(); i++)
            std::cout << "arg[" << i << "]: " << "|" << args[i] << "|" << std::endl;

        execute_cmd(cmd, args, *this, client);
    }
}


// --------------------- //
// Server utils          //
// --------------------- //

int Server::get_client_index(int fd)
{
	for (size_t i = 0; i < _client.size(); i++)
	{
		if (_client[i].get_fd() == fd)
			return (i);
	}
	return (EXIT_FAILURE);
}

int     Server::send_to_client(int fd, std::string msg)
{
    if (send(fd, msg.c_str(), msg.size(), 0) == -1)
    {
        perror("Server send");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

bool    Server::is_nick_free(std::string nickname)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_nickname() == nickname)
            return (false);
    }
    return (true);
}

std::string Server::trim(std::string line)
{
	size_t	first = line.find_first_not_of(" \t");
	size_t	last = line.find_last_not_of(" \t");

    if (last - first <= 0)
        return ("");
	return (line.substr(first, last - first + 1));
}

Client& Server::get_client(int fd)
{
    return (_client[get_client_index(fd)]);
}

std::string Server::get_pass() const
{
    return (_pass);
}

// --------------------- //
// Server exit           //
// --------------------- //

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
