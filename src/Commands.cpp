#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client)
{
    std::string cmd_func[] = {"PASS", "NICK", "USER", "PING", "JOIN", "PART"};
    void    (*fun[])(std::vector<std::string> args, Server &server, Client &client) = {pass, nick, user, ping, join, part};

    for (size_t i = 0; i < 6; i++)
    {
        if (cmd_func[i] == cmd)
            (*fun[i])(args, server, client);
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
    if (args.size() < 2)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "JOIN"));
        return ;
    }
    std::stringstream   stream_channel(args[0]);
    std::stringstream   stream_key(args[1]);
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
        channel[i] = channel[i].substr(1); // Enleve le #
        Channel *c = server.channel_exists(channel[i]);
        if (c == NULL)
        {
            // Create channel
            c->setName(channel[i]);
            if (key.size() > i)
                c->setKey(key[i]);
            c->setOperator(client);
            server.add_channel(*c);
            c->add_client(client);
            server.send_to_client(client.get_fd(), RPL_NOTOPIC(SERVERNAME, client.get_nickname(), c->getName()));
            server.send_to_client(client.get_fd(), RPL_NAMREPLY(SERVERNAME, client.get_nickname(), c->getName(), c->getAllNickname()));
            server.send_to_client(client.get_fd(), RPL_ENDOFNAMES(SERVERNAME, client.get_nickname(), c->getName()));


            // TU ENVOI CA A CHAQUE CLIENTS DU CHANNEL Y COMPRIS LE NOUVEAU UTILISATEUR
            // server.send_to_client(client.get_fd(), JOIN_NOTIFY(SERVERNAME, ....);

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
                if (c->isClientInChannel(client.get_fd()) == true)
                {
                    c->add_client(client);
                    // RPL + NOTIFY
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
        // REMOVE CLIENT FROM CHANNEL
        c->removeClient(client.get_fd());
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
    // SEND NOTIFICATION TO ALL CLIENTS IN CHANNEL
    // SEND RPL TO CLIENT
    // REMOVE CLIENT FROM CHANNEL
    channel->removeClient(client.get_fd());
}

void    privmsg(std::vector<std::string> args, Server &server, Client &client)
{
    if (args.size() < 2)
    {
        server.send_to_client(client.get_fd(), ERR_NEEDMOREPARAMS(SERVERNAME, client.get_nickname(), "PRIVMSG"));
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
            Channel *channel = server.channel_exists(target[i].substr(1));
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
