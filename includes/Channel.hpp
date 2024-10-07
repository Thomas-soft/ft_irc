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
    size_t  _limit;
    Channel();
public:
    Channel(std::string name);
    ~Channel();
    std::string getName() const;
    std::string getKey() const;
	std::string getTopic() const;
    bool    getTopicSet() const;
    bool    getInviteOnly() const;
    size_t  getLimit() const;
	void	removeClient(int fd);
	void    setTopic(std::string topic);
    void    setKey(std::string key);
    void    setName(std::string name);
    void    setOperator(Client& client);
    bool    isClientInChannel(int fd);
    bool    isClientOperator(int fd);
    void    add_client(Client &client);
    std::string    getAllNickname();
};
