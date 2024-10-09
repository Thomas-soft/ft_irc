#include "../includes/Channel.hpp"

Channel::Channel() : _name(""), _key(""), _topic_set(false), _topic(""), _invite_only(false), _limit(0)
{
}

Channel::Channel(std::string name) : _name(name), _key(""), _topic_set(false), _topic(""), _invite_only(false), _limit(0)
{
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
    return (_name);
}

std::string Channel::getKey() const
{
    return (_key);
}

std::string Channel::getTopic() const
{
    return (_topic);
}

bool    Channel::getTopicSet() const
{
    return (_topic_set);
}

bool    Channel::getInviteOnly() const
{
    return (_invite_only);
}

size_t  Channel::getLimit() const
{
    return (_limit);
}

void  Channel::setLimit(size_t newLimit) 
{
    _limit = newLimit;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    _invite_only = inviteOnly;
}

void    Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void    Channel::setKey(std::string key)
{
    _key = key;
}

void    Channel::setOperator(Client& client)
{
    _operator.push_back(client);
}

void    Channel::setUser(Client& client)
{
    for (std::vector<Client>::iterator it = _operator.begin(); it != _operator.end(); it++)
    {
        if (it->get_fd() == client.get_fd())
        {
            _operator.erase(it);
            break ;
        }
    }
    _client.push_back(client);
}

void    Channel::setName(std::string name)
{
    _name = name;
}

bool    Channel::isClientInChannel(int fd)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_fd() == fd)
            return (true);
    }
    return (false);
}

bool    Channel::isClientOperator(int fd)
{
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_client[i].get_fd() == fd)
            return (true);
    }
    return (false);
}

void    Channel::add_client(Client &client)
{
    _client.push_back(client);
}

void	Channel::removeClient(int fd)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_fd() == fd)
            _client.erase(_client.begin() + i);
    }
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_operator[i].get_fd() == fd)
            _operator.erase(_operator.begin() + i);
    }
}

std::string    Channel::getAllNickname()
{
    std::string allNickname;

    for (size_t i = 0; i < _operator.size(); i++)
    {
        allNickname += "@" + _operator[i].get_nickname() + " ";
    }

    for (size_t i = 0; i < _client.size(); i++)
    {
        if (!this->isClientOperator(_client[i].get_fd()))
        {
            if (i + 1 < _client.size())
            {
                allNickname += _client[i].get_nickname();
                return allNickname;
            }
            allNickname += _client[i].get_nickname() + " ";
        }
    }
    return (allNickname);
}

std::vector<Client> Channel::getAllClients() const
{
    return this->_client;
}

Client* Channel::getKickTarget(const std::string &nickname)
{
    for (size_t i = 0; i < _client.size(); i++)
    {
        if (_client[i].get_nickname() == nickname)
            return &_client[i];
    }
    return NULL;
}

bool   Channel::isEmpty()
{
    if (_client.size() == 0)
        return (true);
    return false;
}

void    Channel::sendNotifToAllClients(Server &server, int fd, std::string message, bool sendToAll)
{
    size_t i = 0;

	while (i < _client.size())
	{
        if (fd != _client[i].get_fd() || sendToAll == true)
        {
        server.send_to_client(_client[i].get_fd(), message);
        }
		i++;
	}
}

void Channel::setTopicSet(bool topic_set)
{
    _topic_set = topic_set;
}