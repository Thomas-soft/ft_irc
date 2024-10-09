#pragma once

#include "Server.hpp"

class Server;
class Client;

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client);

void    pass(std::vector<std::string>, Server &server, Client &client);
void    nick(std::vector<std::string>, Server &server, Client &client);
void    user(std::vector<std::string>, Server &server, Client &client);
void    ping(std::vector<std::string>, Server &server, Client &client);
void    join(std::vector<std::string>, Server &server, Client &client);
void    part(std::vector<std::string>, Server &server, Client &client);
void    kick(std::vector<std::string>, Server &server, Client &client);
void    privmsg(std::vector<std::string>, Server &server, Client &client);
void    quit(std::vector<std::string>, Server &server, Client &client);
void    topic(std::vector<std::string>, Server &server, Client &client);
void    invite(std::vector<std::string>, Server &server, Client &client);
void    mode(std::vector<std::string> args, Server &server, Client &client);