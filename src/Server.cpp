#include "../includes/Server.hpp"

bool _signal = false;

Server::Server(int port, std::string pass) : _port(port), _pass(pass), _run(true)
{
}

Server::~Server()
{
}

// --------------------- //
// Server start and loop //
// --------------------- //

void	signalHandler(int signum)
{
	(void) signum;
	_signal = true;
}

void    Server::start()
{
    int opt = 1;

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::signal(SIGINT, signalHandler);
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
    while (_run == true)
    {
        int status = 0;

        status = poll(_pollfd.data(), _pollfd.size(), -1);
        if (status == -1)
            ft_exit("Server poll");
        if (status == 0)
            continue ;
		if (_signal)
			break;
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
    char    *client_ip = inet_ntoa(client_address.sin_addr);
    pollfd  client_pollfd = {client_socket, POLLIN, 0};
    Client  client(client_pollfd.fd, client_ip);
    _pollfd.push_back(client_pollfd);
	_client.push_back(client);
    // Affichage de l'adresse IP du client
    std::cout << "New client connected!" << std::endl;
}

void    Server::read_client(size_t i)
{
    char    buffer[512] = {0};

    std::string         line = buffer;
    std::vector<std::string>    lines;
    size_t  start = 0;
    size_t  end = 0;
    size_t  k = i;


	if (recv(_pollfd[i].fd, buffer, sizeof(buffer), 0) <= 0)
	{
		_client.erase(_client.begin() + get_client_index(_pollfd[i].fd));
		close(_pollfd[i].fd);
		_pollfd.erase(_pollfd.begin() + i);
		std::cout << "Client disconnected!" << std::endl;
		return;
	}
	_client[get_client_index(_pollfd[i].fd)].set_buffer(_client[get_client_index(_pollfd[i].fd)].get_buffer() + buffer);
	line = _client[get_client_index(_pollfd[i].fd)].get_buffer();
	std::cout << _client[get_client_index(_pollfd[i].fd)].get_buffer() << std::endl;
    if (line.find("\n") == std::string::npos)
    	return;
    // std::cout << line << std::endl;
    // std::cout << "Message from client (fd=" << _pollfd[i].fd << "): " << buffer << std::endl;
    // parse(buffer, this->get_client(_pollfd[i].fd));

    while (line.find("\n", start) != std::string::npos)
    {
        end = line.find("\n", start);
        if (end - start > 0)
        {
            lines.push_back(line.substr(start, end - start));
        }
        start = end + 1;
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
        for (size_t i = 0; i < args.size(); i++)
        {
            if (args[i][0] == ':' && args[i].size() != 1)
            {
                size_t  begin = i;
                std::string last_arg = "";
                for (size_t j = i; j < args.size(); j++)
                {
                    if (j == begin)
                        last_arg += args[j].substr(1);
                    else
                        last_arg += args[j];
                    if (j + 1 < args.size())
                        last_arg += " ";
                }
                args.erase(args.begin() + i, args.end());
                args.push_back(last_arg);
                break ;
            }
        }
        // DISPLAY
        if (cmd[0] == '/')
            cmd = cmd.substr(1, cmd.length() - 1);
        std::cout << "cmd: " << "|" << cmd << "|" << std::endl;
        size_t  j = 0;
        for (; j < args.size(); j++);
        if (args.size() > 0 && args[j - 1].size() > 0 && args[j - 1][0] == ':')
            args.pop_back();
        // IL METS UN ARGUMENT VIDE EN TROP LORS DU NICK _
        for (j = 0; j < args.size(); j++)
            std::cout << "arg[" << j << "]: " << "|" << args[j] << "|" << std::endl;
        execute_cmd(cmd, args, *this, this->get_client(_pollfd[k].fd));
        std::cout << "Client connecter: " << _client.size() << std::endl;
		_client[get_client_index(_pollfd[k].fd)].set_buffer(std::string(""));
    }



	// if (_client.size())
	// 	std::cout << "\nTous les clients" << std::endl;
	// for (size_t i = 0; i < _client.size(); i++)
	// {
	// 	std::cout << "Client " << i << ": " << _client[i].get_nickname() << std::endl;
	// }
	// if (_channel.size())
	// 	std::cout << "\nTous les channel" << std::endl;
	// for (size_t i = 0; i < _channel.size(); i++)
	// {
	// 	std::cout << "Channel " << i << ": " << _channel[i].getName() << " avec " << _channel[i].getAllClients().size() << " clients" << std::endl;
	// }
}


// --------------------- //
// Server parsing        //
// --------------------- //

// void    Server::parse(char *buffer, Client &client)
// {
//     // execute_cmd(cmd, args, *this, client);
//     (void)buffer;
//     (void)client;

//     std::string         line = buffer;
//     std::vector<std::string>    lines;
//     size_t  start = 0;
//     size_t  end = 0;

//     while (line.find("\n", start) != std::string::npos)
//     {
//         end = line.find("\n", start);
//         if (end - start > 0)
//             lines.push_back(line.substr(start, end - start));
//         start = end + 1;
//     }
//     for (size_t i = 0; i < lines.size(); i++)
//     {
//         lines[i] = trim(lines[i]);
//         std::string cmd = "";
//         std::vector<std::string>    args;
//         std::stringstream   stream(lines[i]);
//         stream >> cmd;
//         if (cmd == "")
//             continue ;
//         std::string word = "";
//         std::stringstream   args_stream(stream.str().substr(cmd.size()));
//         while (args_stream >> word)
//             args.push_back(word);
//         for (size_t i = 0; i < args.size(); i++)
//         {
//             if (args[i][0] == ':' && args[i].size() != 1)
//             {
//                 size_t  begin = i;
//                 std::string last_arg = "";
//                 for (size_t j = i; j < args.size(); j++)
//                 {
//                     if (j == begin)
//                         last_arg += args[j].substr(1);
//                     else
//                         last_arg += args[j];
//                     if (j + 1 < args.size())
//                         last_arg += " ";
//                 }
//                 args.erase(args.begin() + i, args.end());
//                 args.push_back(last_arg);
//                 break ;
//             }
//         }
//         // DISPLAY
// 		if (cmd[0] == '/')
// 			cmd = cmd.substr(1, cmd.length() - 1);
//         std::cout << "cmd: " << "|" << cmd << "|" << std::endl;
//         size_t  j = 0;
//         for (; j < args.size(); j++);
//         if (args.size() > 0 && args[j - 1].size() > 0 && args[j - 1][0] == ':')
//             args.pop_back();
//         for (j = 0; j < args.size(); j++)
//             std::cout << "arg[" << j << "]: " << "|" << args[j] << "|" << std::endl;
//         execute_cmd(cmd, args, *this, client);
//     }
// }


// --------------------- //
// Server utils          //
// --------------------- //

void Server::remove_channel(std::string name)
{
    for (size_t i = 0; i < _channel.size(); i++)
    {
        if (_channel[i].getName() == name)
        {
            _channel.erase(_channel.begin() + i);
            return ;
        }
    }
}

void	Server::remove_client(int fd)
{
	size_t i = 0;

	while (i < _client.size())
	{
		if (_client[i].get_fd() == fd)
        {
			_client.erase(_client.begin() + i);
			_pollfd.erase(_pollfd.begin() + i + 1);
        }
        else
		    i++;
	}
}

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

bool    Server::is_nick_free(std::string nickname, int fd)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_fd() != fd && _client[i].get_nickname() == nickname && _client[i].is_registered() == true)
            return (false);
        if (_client[i].get_fd() != fd)
        {
            std::cout << "NICKNAME:" << _client[i].get_nickname() << std::endl;
            std::cout << "NEW_NICKNAME:" << nickname << std::endl;
            std::cout << std::endl;
        }
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

Channel*    Server::channel_exists(std::string name)
{
    for (size_t i = 0; i < _channel.size(); i++)
    {
        if (_channel[i].getName() == name)
            return (&_channel[i]);
    }
    return (NULL);
}

Client* Server::client_exists(std::string nickname)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_nickname() == nickname)
            return (&_client[i]);
    }
    return (NULL);
}

void    Server::add_channel(Channel& channel)
{
    _channel.push_back(channel);
}

void    Server::add_client_to_channel(Client& client, std::string channelName)
{
    for (size_t i = 0; i < _channel.size(); i++)
    {
        if (_channel[i].getName() == channelName)
        {
            _client.clear();
            _channel[i].add_client(client);
            return ;
        }
    }
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
        if (_pollfd[i].fd >= 0)
            close(_pollfd[i].fd);
    }
    // close(_server_fd);
}

void    Server::ft_exit(std::string error)
{
    perror(error.c_str());
	close_all_fd();
    // delete _pollfd.data();
    // delete _client.data();
    // delete _channel.data();
    std::vector<Channel>&    channels = this->getAllChannels();

    for (size_t i = 0; i < channels.size(); i++)
    {
        channels[i].clear_client();
    }

	_client.clear();
	_pollfd.clear();
    _channel.clear();
    _run = false;
    // exit(EXIT_FAILURE);
}

void Server::delete_client(int fd)
{
    std::cout << "Client size: " << _client.size() << std::endl;
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_fd() == fd)
        {
            std::cout << "Client size: " << _client.size() << std::endl;
            _client.erase(_client.begin() + i);
            std::cout << "Client size: " << _client.size() << std::endl;
            return ;
        }
    }
}

void Server::delete_poll_client(int fd)
{
    std::cout << "pollfd size : " << _pollfd.size() << std::endl;
    for (size_t i = 0; i < _pollfd.size(); i++)
    {
        if (_pollfd[i].fd == fd)
        {
            std::cout << "pollfd size : " << _pollfd.size() << std::endl;
            _pollfd.erase(_pollfd.begin() + i);
            std::cout << "pollfd size : " << _pollfd.size() << std::endl;
            return ;
        }
    }
}

std::vector<Channel>& Server::getAllChannels()
{
    return (_channel);
}