#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client)
{
    std::string cmd_func[] = {"PING", "JOIN", "PART", "KICK", "PRIVMSG", "QUIT", "TOPIC", "INVITE"};
	std::string cmd_func_unregistred[] = {"PASS", "NICK", "USER"};
    void    (*fun[])(std::vector<std::string> args, Server &server, Client &client) = {pass, nick, user, ping, join, part, kick, privmsg, quit, topic, invite};

    for (size_t i = 0; i < 3; i++)
    {
        if (cmd_func_unregistred[i] == cmd)
            (*fun[i])(args, server, client);
    }
    for (size_t i = 0; i < 8; i++)
    {
        if (client.is_registered() && cmd_func[i] == cmd)
            (*fun[i + 3])(args, server, client);
    }
    if (client.is_registered() == false
        && client.get_password() != ""
        && client.get_nickname() != ""
        && client.get_username() != ""
        && client.get_hostname() != ""
        && client.get_servername() != ""
        && client.get_realname() != "")
    {
        server.send_to_client(client.get_fd(), RPL_WELCOME(SERVERNAME, client.get_nickname()));
        server.send_to_client(client.get_fd(), RPL_YOURHOST(SERVERNAME, client.get_nickname()));
        server.send_to_client(client.get_fd(), RPL_CREATED(SERVERNAME, client.get_nickname()));
        server.send_to_client(client.get_fd(), RPL_MYINFO(SERVERNAME, client.get_nickname(), "1"));
        server.send_to_client(client.get_fd(), RPL_ISUPPORT(SERVERNAME, client.get_nickname()));
        client.set_registered(true);
    }
}

void    pass(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 1)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "PASS"));
        return ;
    }
    if (client.is_registered())
    {
        server.send_to_client(client.get_fd(), ERR_ALREADYREGISTRED(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args[0] != server.get_pass())
    {
        server.send_to_client(client.get_fd(), ERR_PASSWDMISMATCH(SERVERNAME, client.get_nickname()));
        return ;
    }
    client.set_password(args[0]);
}

void    nick(std::vector<std::string> args, Server &server, Client &client)
{
    if (client.get_password() == "")
    {
        server.send_to_client(client.get_fd(), ERR_PASSWDMISMATCH(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() == 0)
    {
        server.send_to_client(client.get_fd(), ERR_NONICKNAMEGIVEN(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() > 0)
    {
        if (args[0].size() > 9)
        {
            server.send_to_client(client.get_fd(), ERR_ERRONEUSNICKNAME(SERVERNAME, client.get_nickname()));
            return ;
        }
        if (server.is_nick_free(args[0]) == false)
        {
            server.send_to_client(client.get_fd(), ERR_NICKNAMEINUSE(SERVERNAME, client.get_nickname()));
            return ;
        }
        client.set_nickname(args[0]);
    }
}

void    user(std::vector<std::string> args, Server &server, Client &client)
{
    if (client.is_registered())
    {
        server.send_to_client(client.get_fd(), ERR_ALREADYREGISTRED(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (client.get_password() == "")
    {
        server.send_to_client(client.get_fd(), ERR_PASSWDMISMATCH(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (client.get_nickname() == "")
    {
        server.send_to_client(client.get_fd(), ERR_NONICKNAMEGIVEN(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() < 4)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "USER"));
        return ;
    }
    if (client.get_username() != "")
    {
        server.send_to_client(client.get_fd(), ERR_ALREADYREGISTRED(SERVERNAME, client.get_nickname()));
        return ;
    }
    client.set_username(args[0]);
    client.set_hostname("0");
    client.set_servername("*");
    client.set_realname(args[3]);
}

void    ping(std::vector<std::string> args, Server &server, Client &client)
{
	(void)args;
	if (args.size() == 0)
    {
		server.send_to_client(client.get_fd(), ERR_NOORIGIN(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args[0] != SERVERNAME)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHSERVER(SERVERNAME, client.get_nickname()));
        return ;
    }
	server.send_to_client(client.get_fd(), RPL_PING(SERVERNAME));
}

void    join(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 1)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "JOIN"));
        return ;
    }
    std::stringstream   stream_channel(args[0]);
    std::string keys = "";
    if (args.size() > 1)
        keys = args[1];
    std::stringstream   stream_key(keys);
    std::vector<std::string>    channel;
    std::vector<std::string>    key;
    std::string                 token = "";

    while (std::getline(stream_channel, token, ','))
        channel.push_back(token);
    while (std::getline(stream_key, token, ','))
        key.push_back(token);

    if (channel.size() > 10)
    {
        server.send_to_client(client.get_fd(), ERR_TOOMANYCHANNELS(SERVERNAME, client.get_nickname()));
        return ;
    }
    for (size_t i = 0; i < channel.size(); i++)
    {
        if (channel[i][0] != '#')
        {
            server.send_to_client(client.get_fd(), ERR_BADCHANMASK(SERVERNAME, client.get_nickname()));
        }
        Channel *c = server.channel_exists(channel[i]);
        if (c == NULL)
        {
            // Create channel
            Channel new_channel(channel[i]);
            if (key.size() > i)
                new_channel.setKey(key[i]);
            new_channel.setOperator(client);
            new_channel.add_client(client);
            server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(),  new_channel.getName()));
            server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), new_channel.getName(), new_channel.getAllNickname()));
            server.send_to_client(client.get_fd(), RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), new_channel.getName()));
            server.add_channel(new_channel);
            //std::cout << RPL_NOTOPIC(SERVERNAME, client.get_nickname(), new_channel.getName()) << "|" << std::endl;
            //std::cout << RPL_NAMREPLY(SERVERNAME, client.get_nickname(), new_channel.getName(), new_channel.getAllNickname()) << std::endl;
            //std::cout << RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), new_channel.getName()) << std::endl;
            // mis à jour le send_to_server par Dylan. vérifier si c'est bon

        }
        else
        {
            if (key.size() > i)
            {
                if (c->getKey() != "" && c->getKey() != key[i])
                {
                    server.send_to_client(client.get_fd(), ERR_BADCHANNELKEY(SERVERNAME, client.get_nickname()));
                }
            }
            else
            {
                // Add client to channel
                // server.add_client_to_channel(client, channel[i]);
                if (c->isClientInChannel(client.get_fd()) == false)
                {
                    c->add_client(client);
                    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL à vérif
                    std::vector<Client> clients = c->getAllClients();
                    for (size_t i = 0; i < clients.size(); i++)
                        server.send_to_client(clients[i].get_fd(), JOIN_NOTIFY(clients[i].get_nickname(), clients[i].get_username(), clients[i].get_hostname(), c->getName()));
                }
            }
        }
    }
}

void    part(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() == 0)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "PART"));
        return ;
    }
    std::stringstream   stream_channel(args[0]);
    std::vector<std::string>    channel;
    std::string                 token;

	while (std::getline(stream_channel, token, ','))
		channel.push_back(token);
    for (size_t i = 0; i < channel.size(); i++)
    {
        Channel *c = server.channel_exists(channel[i]);
        if (c == NULL)
        {
            server.send_to_client(client.get_fd(), ERR_NOSUCHCHANNEL(SERVERNAME, client.get_nickname()));
            return ;
        }
        if (c->isClientInChannel(client.get_fd()) == false)
        {
            server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
            return ;
        }
        // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
        // SEND RPL TO CLIENT
        // remove channel if empty
        c->removeClient(client.get_fd());
        if (c->isEmpty())
            server.remove_channel(c->getName());
		else
			c->sendQuitMessage(server, client.get_fd());
		//std::cout << RPL_PART(client.get_nickname(), client.get_username(), "127.0.0.1", c->getName()) << std::endl;
        server.send_to_client(client.get_fd(), RPL_PART(client.get_nickname(), client.get_username(), "127.0.0.1", c->getName()));
        //std::cout << PART_NOTIFY(client.get_nickname(), client.get_username(), "localhost", c->getName(), "Goodbye!") << std::endl;
        // server.send_to_client(client.get_fd(), RPL_PART(SERVERNAME, client.get_nickname(), c->getName()));  
        // REMOVE CLIENT FROM CHANNEL (and operators)
        // display all clients in channel
    }
}

void    kick(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 2)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "KICK"));
        return ;
    }
    Channel *channel = server.channel_exists(args[0]);
    if (channel == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (channel->isClientInChannel(client.get_fd()) == false) 
    {
        server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (channel->isClientOperator(client.get_fd()) == false)
    {
        server.send_to_client(client.get_fd(), ERR_CHANOPRIVSNEEDED(SERVERNAME, client.get_nickname()));
        return ;
    }
    // mis à jour le send_to_server par Dylan. vérifier si c'est bon
    Client *target = channel->getKickTarget(args[1]);
    if (target == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
        return ;
    }
    
    std::vector<Client> clients = channel->getAllClients();
    for (size_t i = 0; i < clients.size(); i++)
        server.send_to_client(clients[i].get_fd(), KICK_NOTIFY(clients[i].get_nickname(), clients[i].get_username(), clients[i].get_hostname(), channel->getName(), target->get_nickname(), args[2]));
    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
    // SEND RPL TO CLIENT
    // REMOVE CLIENT FROM CHANNEL (and operators)
    channel->removeClient(client.get_fd());
}

void    privmsg(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() == 0)
    {
        server.send_to_client(client.get_fd(), ERR_NORECIPIENT(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() == 1)
    {
        if (args[0][0] == ':')
        {
            server.send_to_client(client.get_fd(), ERR_NORECIPIENT(SERVERNAME, client.get_nickname()));
            return ;
        }
        server.send_to_client(client.get_fd(), ERR_NOTEXTTOSEND(SERVERNAME, client.get_nickname()));
        return ;
    }
    std::stringstream           stream_channel(args[0]);
    std::vector<std::string>    target;
    std::string                 token;

    while (std::getline(stream_channel, token, ','))
        target.push_back(token);
    for (size_t i = 0; i < target.size(); i++)
    {
        if (target[i][0] == '#')
        {
            Channel *channel = server.channel_exists(target[i]);
            if (channel == NULL)
            {
                server.send_to_client(client.get_fd(), ERR_NOSUCHCHANNEL(SERVERNAME, client.get_nickname()));
                return ;
            }
            if (channel->isClientInChannel(client.get_fd()) == false)
            {
                server.send_to_client(client.get_fd(), ERR_CANNOTSENDTOCHAN(SERVERNAME, client.get_nickname(), channel->getName()));
                return ;
            }
            // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
            // SEND RPL TO CLIENT
        }
        else
        {
            Client *c = server.client_exists(target[i]);
            if (c == NULL)
            {
                server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
                return ;
            }
            // SEND NOTIFICATION TO CLIENT
            // SEND RPL TO CLIENT
        }
    }
}

void    quit(std::vector<std::string> args, Server &server, Client &client)
{
    (void)args;
    (void)server;
    (void)client;

	std::cout << "hey" << std::endl;
	server.remove_client(client.get_fd());
    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
    server.send_to_client(client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), "Goodbye!"));
    // REMOVE CLIENT FROM CHANNEL (channel>clients, channel>operators)
    // REMOVE CLIENT FROM SERVER (pollfd, client)
}

void    topic(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 1)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "TOPIC"));
        return ;
    }
    Channel *channel = server.channel_exists(args[0]);
    if (channel == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (channel->isClientInChannel(client.get_fd()) == false)
    {
        server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() == 1)
    {
        if (channel->getTopic() == "")
        {
            server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(), channel->getName()));
            return ;
        }
        server.send_to_client(client.get_fd(), RPL_TOPIC(SERVERNAME, client.get_nickname(), channel->getName(), channel->getTopic()));
        return ;
    }
    if (channel->isClientOperator(client.get_fd()) == false && channel->getTopicSet() == false)
    {
        server.send_to_client(client.get_fd(), ERR_CHANOPRIVSNEEDED(SERVERNAME, client.get_nickname()));
        return ;
    }
    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
    // SEND RPL TO CLIENT
    // SET NEW TOPIC
}

void    invite(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 2)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "INVITE"));
        return ;
    }
    Client *c = server.client_exists(args[0]);
    if (c == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
        return ;
    }
    Channel *channel = server.channel_exists(args[1]);
    if (channel == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (channel->isClientInChannel(client.get_fd()) == false)
    {
        server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (channel->isClientOperator(client.get_fd()) == false)
    {
        server.send_to_client(client.get_fd(), ERR_CHANOPRIVSNEEDED(SERVERNAME, client.get_nickname()));
        return ;
    }
    // if invite only
    // if (channel->getInviteOnly())
}

// DANS JOIN NE PAS OUBLIER LA LIMITE DU CHANNEL SI ELLE N'EST PAS A 0