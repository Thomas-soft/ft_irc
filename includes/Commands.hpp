#pragma once

#include "Server.hpp"

class Server;

void    execute_cmd(std::string cmd, std::string args, Server &server);
void    pass(std::string args, Server &server);
