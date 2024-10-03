#pragma once

#include "Server.hpp"

class Client
{
private:
    int _fd;
    std::string _user;
    std::string _nick;
    std::string _pass;
    bool        registred;
    

public:
    Client(int fd, std::string nick, std::string user, std::string pass);
    Client(int fd);
    ~Client();
};
