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
#include "rpl.hpp"
#include <csignal>
#include <map>
#include "Channel.hpp"

#define SERVERNAME "ft_irc.com"


class Channel;
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
        std::vector<Channel>    _channel;
        Server();

    public:
        Server(int port, std::string pass);
        ~Server();

        // --------------------- //
        // Server start and loop //
        // --------------------- //
        void    start();
        void    accept_client();
        void    read_client(size_t i);
		//void	signalHandler(int signum);

        // --------------------- //
        // Server parsing        //
        // --------------------- //
        void    parse(char *buffer, Client &client);

        // --------------------- //
        // Server utils          //
        // --------------------- //
        int		get_client_index(int fd);
        int     send_to_client(int fd, std::string msg);
        bool    is_nick_free(std::string nick);
        std::string trim(std::string line);
        Channel*    channel_exists(std::string name);
        Client*     client_exists(std::string name);
		void	remove_client(int fd);
        void    add_channel(Channel &channel);
        void    add_client_to_channel(Client &client, std::string channelName);
        Client& get_client(int fd);
        std::string get_pass() const;
		
        // --------------------- //
        // Server exit           //
        // --------------------- //
        void    ft_exit(std::string error);
        void    close_all_fd();
};
