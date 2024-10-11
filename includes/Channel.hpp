#pragma once

#include "Server.hpp"

class Server;

class Channel
{
private:
    std::string _name;
    std::string _key;
    std::vector<Client> _client;
    std::vector<Client> _operator;
    std::vector<Client> _invited;
    bool _topic_set;
    std::string _topic;
    bool _invite_only;
    size_t  _limit;
    Channel();

public:
    Channel(std::string name);
    ~Channel();

    // ---------------------- //
	//   Channel setter       //
	// ---------------------- //
	void    setTopic(std::string topic);
    void    setKey(std::string key);
    void    setName(std::string name);
    void    setOperator(Client& client);
    void    setUser(Client& client);
    void    setInviteOnly(bool inviteOnly);
    void    setLimit(size_t newLimit); 
    std::vector<Client> getAllClients() const;
    std::string    getAllNickname();

    // ---------------------- //
	//   Channel getters      //
	// ---------------------- //
    std::string getName() const;
    std::string getKey() const;
	std::string getTopic() const;
    bool    getTopicSet() const;
    bool    getInviteOnly() const;
    size_t  getLimit() const;

    // ---------------------- //
	//   Other function       //
	// ---------------------- //
	void	removeClient(int fd);
	void	sendNotifToAllClients(Server &server, int fd, std::string message, bool sendToAll);
    bool    isClientInChannel(int fd);
    bool    isClientOperator(int fd);
    void    add_client(Client &client);
    Client* getKickTarget(const std::string &nickname);
    bool    isEmpty();
    void    setTopicSet(bool topic_set);
    void    push_invited(Client &client);
    bool    isInvited(Client &client);
    void    removeInvited(Client &client);
    void    clear_client();
};
