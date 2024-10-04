#pragma once

#include "Server.hpp"

class Server;
class Client;

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client);

void    pass(std::vector<std::string>, Server &server, Client &client);
void    nick(std::vector<std::string>, Server &server, Client &client);
void    user(std::vector<std::string>, Server &server, Client &client);
