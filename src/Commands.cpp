#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::string args, Server &server)
{
    std::string cmd_func[] = {"PASS"};
    void    (*fun[])(std::string args, Server &server) = {pass};

    for (size_t i = 0; i < 1; i++)
    {
        if (cmd_func[i] == cmd)
            (*fun[i])(args, server);
    }
}

void    pass(std::string args, Server &server)
{
    (void)args;
    (void)server;
}

// Dans la commande MSG on aura besoin de recuperer tout les clients dispo dans un channel

//  on aura besoin donc du channel et de tout les clients
// Enfaite faut toutes les infos, meme ceux du serveur, TOUT
