#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::vector<std::string> args, Server &server, Client &client)
{
    std::string cmd_func[] = {"PASS", "NICK", "USER", "PING"};
    void    (*fun[])(std::vector<std::string> args, Server &server, Client &client) = {pass, nick, user, ping};

    for (size_t i = 0; i < 4; i++)
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
