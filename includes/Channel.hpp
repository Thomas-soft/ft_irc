#pragma once

#include "Server.hpp"

class Channel
{
private:
    std::string _name;
    std::string _key;
    std::vector<Client> _client;
    std::vector<Client> _operator;
    bool _topic_set;
    std::string _topic;
    bool _invite_only;
    int _limit;
public:
    Channel();
    ~Channel();
};
