#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client)
{
    std::string cmd_func[] = {"PING", "JOIN", "PART", "KICK", "PRIVMSG", "QUIT", "TOPIC", "INVITE", "MODE"};
	std::string cmd_func_unregistred[] = {"PASS", "NICK", "USER"};
    void    (*fun[])(std::vector<std::string> args, Server &server, Client &client) = {pass, nick, user, ping, join, part, kick, privmsg, quit, topic, invite, mode};

    for (size_t i = 0; i < 3; i++)
    {
        if (cmd_func_unregistred[i] == cmd)
            (*fun[i])(args, server, client);
    }
    for (size_t i = 0; i < 9; i++)
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
        if (server.is_nick_free(args[0], client.get_fd()) == false)
        {
            server.send_to_client(client.get_fd(), ERR_NICKNAMEINUSE(SERVERNAME, args[0]));
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
    client.set_hostname("127.0.0.1");
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
        std::cout << "11111111111111111111111111111" << std::endl;
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

    // if (channel.size() > 10)
    // {
    //     server.send_to_client(client.get_fd(), ERR_TOOMANYCHANNELS(SERVERNAME, client.get_nickname()));
    //     return ;
    // }
    for (size_t i = 0; i < channel.size(); i++)
    {
        if (channel[i][0] != '#' && channel[i].size() != 1)
        {
            server.send_to_client(client.get_fd(), ERR_BADCHANMASK(SERVERNAME, client.get_nickname()));
            continue ;
        }
        Channel *c = server.channel_exists(channel[i]);
        // vérifier si le salon est sur invitation
        if (c == NULL)
        {
            // Create channel
            Channel new_channel(channel[i]);
            new_channel.setOperator(client);
            new_channel.add_client(client);
            if (key.size() > i)
                new_channel.setKey(key[i]);
            server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(),  new_channel.getName()));
            server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), new_channel.getName(), new_channel.getAllNickname()));
            server.send_to_client(client.get_fd(), RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), new_channel.getName()));
            server.add_channel(new_channel);
            new_channel.sendNotifToAllClients(server, client.get_fd(), JOIN_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), new_channel.getName()), true);
            continue ;
        }
        else if (c->getInviteOnly() == true && c->isInvited(client) == false)
        {
            server.send_to_client(client.get_fd(), ERR_INVITEONLYCHAN(SERVERNAME, client.get_nickname()));
            return ;
        }
        else if (c->getLimit() != 0 && c->getLimit() <= c->getAllClients().size())
        {
            server.send_to_client(client.get_fd(), ERR_CHANNELISFULL(SERVERNAME, client.get_nickname()));
            return ;
        }
        else
        {
            if (c->getKey() != "")
            {
                if (key.size() > i && key[i] == c->getKey())
                {
                    std::cout << "dsadwadsadwadsadwadsadwadsadwadsadawdsadsa" << std::endl;
                    if (c->isClientInChannel(client.get_fd()) == false)
                    {
                        if (c->getLimit() != 0 && c->getLimit() <= c->getAllClients().size())
                        {
                            server.send_to_client(client.get_fd(), ERR_CHANNELISFULL(SERVERNAME, client.get_nickname()));
                            continue ;
                        }
                        c->removeInvited(client);
                        c->add_client(client);
                        // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL à vérif
                        std::vector<Client> clients = c->getAllClients();
                        if (c->getTopic() != "")
                            server.send_to_client(client.get_fd(), RPL_TOPIC(SERVERNAME, client.get_nickname(), c->getName(), c->getTopic()));
                        else
                            server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(),  c->getName()));
                        server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), c->getName(), c->getAllNickname()));
                        server.send_to_client(client.get_fd(), RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), c->getName()));
                        c->sendNotifToAllClients(server, client.get_fd(), JOIN_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName()), true);
                    }
                }
                else
                {
                    server.send_to_client(client.get_fd(), ERR_BADCHANNELKEY(SERVERNAME, client.get_nickname()));
                    continue ;
                }
            }
            else
            {
                // Add client to channel
                // server.add_client_to_channel(client, channel[i]);
                if (c->isClientInChannel(client.get_fd()) == false)
                {
                    if (c->getLimit() != 0 && c->getLimit() <= c->getAllClients().size())
                    {
                        server.send_to_client(client.get_fd(), ERR_CHANNELISFULL(SERVERNAME, client.get_nickname()));
                        return ;
                    }
                    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL à vérif
                    std::vector<Client> clients = c->getAllClients();
                    if (c->getTopic() != "")
                        server.send_to_client(client.get_fd(), RPL_TOPIC(SERVERNAME, client.get_nickname(), c->getName(), c->getTopic()));
                    else
                        server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(),  c->getName()));
                    server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), c->getName(), c->getAllNickname()));
                    server.send_to_client(client.get_fd(), RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), c->getName()));
                    c->sendNotifToAllClients(server, client.get_fd(), JOIN_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName()), true);
                    c->removeInvited(client);
                    c->add_client(client);
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
            continue ;
        }
        if (c->isClientInChannel(client.get_fd()) == false)
        {
            server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
            continue ;
        }
        if (args.size() > 1)
            c->sendNotifToAllClients(server, client.get_fd(), PART_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), args[1]), true);
        else
		    c->sendNotifToAllClients(server, client.get_fd(), PART_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "Goodbye!"), true);
        std::cout << "Client en moins..." << std::endl;
        c->removeClient(client.get_fd());
        if (c->isEmpty())
            server.remove_channel(c->getName());
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
    Client *target = channel->getKickTarget(args[1]);
    if (target == NULL)
    {
        server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
        return ;
    }

    if (target->get_fd() != client.get_fd())
    {
        if (args.size() > 2)
            channel->sendNotifToAllClients(server, client.get_fd(), KICK_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), channel->getName(), target->get_nickname(), args[2]), true);
        else
            channel->sendNotifToAllClients(server, client.get_fd(), KICK_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), channel->getName(), target->get_nickname(), "Goodbye!"), true);
        channel->removeClient(target->get_fd());
    }
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
                continue ;
            }
            if (channel->isClientInChannel(client.get_fd()) == false)
            {
                server.send_to_client(client.get_fd(), ERR_CANNOTSENDTOCHAN(SERVERNAME, client.get_nickname(), channel->getName()));
                continue ;
            }
            channel->sendNotifToAllClients(server, client.get_fd(), PRIVMSG_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), channel->getName(), args[1]), false);
        }
        else
        {
            Client *c = server.client_exists(target[i]);
            if (c == NULL)
            {
                server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
                continue ;
            }
            std::cout << c->get_fd() << " | " << client.get_fd() << std::endl;
            if (c->get_fd() != client.get_fd())
            {
            server.send_to_client(c->get_fd(), PRIVMSG_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->get_nickname(), args[1]));
            }
        }
    }
}

void    quit(std::vector<std::string> args, Server &server, Client &client)
{
    std::vector<Channel>&  channels = server.getAllChannels();

    for (std::vector<class Channel>::iterator it = channels.begin(); it != channels.end();)
	{
		if (it->isClientInChannel(client.get_fd()))
		{
            std::string reason = "Goodbye";
            if (args.size() > 0)
                reason = args[0];
            it->sendNotifToAllClients(server, client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), reason), false);
			it->removeClient(client.get_fd());
		}
		if (it->isEmpty())
			it = channels.erase(it);
		else
			it++;
	}
    int fd = client.get_fd();
    server.delete_poll_client(fd);
    server.delete_client(fd);
	close(fd);





    // std::vector<Channel> channels = server.getAllChannels();

    // for (size_t i = 0; i < channels.size(); i++)
    // {
    //     std::cout << channels[i].getName() << std::endl;
    //     std::cout << client.get_fd() << std::endl;
    //     if (channels[i].isClientInChannel(client.get_fd()) == true)
    //     {
    //         if (args.size() > 0)
    //             channels[i].sendNotifToAllClients(server, client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), args[0]), true);
    //         else
    //             channels[i].sendNotifToAllClients(server, client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), "Goodbye!"), true);
    //         channels[i].removeClient(client.get_fd());
    //         if (channels[i].isEmpty())
    //         {
    //             server.remove_channel(channels[i].getName());
    //         }
    //     }
    // }
    // std::cout << "QUUUUUUUUUUUUUUUUUUUUUIT" << std::endl;
    // int f = client.get_fd();
    // std::cout << "f: " << f << std::endl;
    // close(client.get_fd());
    // server.delete_client(f);
    // server.delete_poll_client(f);

    // for (it = channels.begin(); it != channels.end();)
    // {
    //     if (it->isClientInChannel(client.get_fd()))
    //     {
    //         it->removeClient(client.get_fd());
    //         if (args.size() > 0)
    //             it->sendNotifToAllClients(server, client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), args[0]), false);
    //         else
    //             it->sendNotifToAllClients(server, client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), "Goodbye!"), false);
    //         if (it->isEmpty())
    //         {
    //             server.remove_channel(it->getName());
    //             it = channels.erase(it);
    //         }
    //         else
    //             ++it;
    //     }
    //     else
    //         ++it;
    // }
    // server.delete_client(client.get_fd());
    // server.delete_poll_client(client.get_fd());
    // server.send_to_client(client.get_fd(), QUIT_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), args[0]));
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
    channel->setTopic(args[1]);
    channel->sendNotifToAllClients(server, client.get_fd(), TOPIC_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), channel->getName(), args[1]), true);
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
    if (channel->getInviteOnly() == false)
    {
        server.send_to_client(client.get_fd(), ERR_INVITEONLYCHAN(SERVERNAME, client.get_nickname()));
        return ;
    }
    // algorithm commande invite
    server.send_to_client(client.get_fd(), RPL_INVITING(SERVERNAME, client.get_nickname(), c->get_nickname(), channel->getName()));
    server.send_to_client(c->get_fd(), INVITE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), channel->getName()));
    channel->push_invited(*c);
}

// DANS JOIN NE PAS OUBLIER LA LIMITE DU CHANNEL SI ELLE N'EST PAS A 0

void    mode(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 2)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "MODE"));
        return ;
    }
    std::string channel = args[0];

    Channel *c = server.channel_exists(channel);
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
    if (c->isClientOperator(client.get_fd()) == false)
    {
        server.send_to_client(client.get_fd(), ERR_CHANOPRIVSNEEDED(SERVERNAME, client.get_nickname()));
        return ;
    }
    if (args.size() > 1 && args[1].size() == 2 && args[1][0] == '+')
    {
        if (args[1][1] == 'i')
        {
            c->setInviteOnly(true);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "+i"), true);
        }
        else if (args[1][1] == 'o')
        {
            
            if (args.size() < 3)
            {
                server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "MODE"));
                return ;
            }
            Client *target = c->getKickTarget(args[2]);
            if (target == NULL)
            {
                server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
                return ;
            }
            if (c->isClientInChannel(target->get_fd()) == false)
            {
                server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
                return ;
            }
            c->setOperator(*target);
            // on doit afficher @ au début du nouveau opérateur ????
            server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), c->getName(), c->getAllNickname()));
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "+o"), true);
        }
        else if (args[1][1] == 'l')
        {
            size_t  newLimit;

            if (args.size() < 3)
            {
                server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "MODE"));
                return ;
            }
            std::stringstream ss(args[2]);
            ss >> newLimit;
            if (ss.fail() || (int)newLimit < 0 || !ss.eof())
            {
                std::cout << "caca boudin" << std::endl;
                return ;
            }
            c->setLimit(newLimit);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "+l"), true);
        }
        else if (args[1][1] == 'k')
        {
            if (args.size() < 3)
            {
                server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "MODE"));
                return ;
            }
            if (c->getKey() != "")
            {
                server.send_to_client(client.get_fd(), ERR_KEYSET(SERVERNAME, c->getName()));
                return ;
            }
            c->setKey(args[2]);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "+k"), true);
        }
        // mode +t pour activé la restriction de la commande topic au operator
        else if (args[1][1] == 't')
        {
            c->setTopicSet(false);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "+t"), true);
        }
        else
        {
            server.send_to_client(client.get_fd(), ERR_UNKNOWNMODE(SERVERNAME, client.get_nickname()));
            return ;
        }
    }
    else if (args.size() > 1 && args[1].size() == 2 && args[1][0] == '-')
    {
        if (args[1][1] == 'i')
        {
            c->setInviteOnly(false);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "-i"), true);
        }
        else if (args[1][1] == 'o')
        {
            if (args.size() < 3)
            {
                server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "MODE"));
                return ;
            }
            Client *target = c->getKickTarget(args[2]);
            if (target == NULL)
            {
                server.send_to_client(client.get_fd(), ERR_NOSUCHNICK(SERVERNAME, client.get_nickname()));
                return ;
            }
            if (c->isClientInChannel(target->get_fd()) == false)
            {
                server.send_to_client(client.get_fd(), ERR_NOTONCHANNEL(SERVERNAME, client.get_nickname()));
                return ;
            }
            c->setUser(*target);
            server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), c->getName(), c->getAllNickname()));
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "-o"), true);
        }
        else if (args[1][1] == 'l')
        {
            c->setLimit(0);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "-l"), true);
        }
        else if (args[1][1] == 'k')
        {
            c->setKey("");
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "-k"), true);
        }
        else if (args[1][1] == 't')
        {
            c->setTopicSet(true);
            c->sendNotifToAllClients(server, client.get_fd(), MODE_NOTIFY(client.get_nickname(), client.get_username(), client.get_hostname(), c->getName(), "-t"), true);
        }
        else
        {
            server.send_to_client(client.get_fd(), ERR_UNKNOWNMODE(SERVERNAME, client.get_nickname()));
            return ;
        }
    }
    else
    {
        server.send_to_client(client.get_fd(), ERR_UNKNOWNMODE(SERVERNAME, client.get_nickname()));
        return ;
    }
}
