#include "../includes/Commands.hpp"

void    execute_cmd(std::string cmd, std::string args, size_t i)
{
    std::string cmd_func[] = {"PASS"};
    void    (*fun[])(std::string args, size_t i) = {pass};

    for (size_t j = 0; j < 1; j++)
    {
        if (cmd_func[j] == cmd)
            (*fun[j])(args, i);
    }
}

void    pass(std::string args, size_t i)
{
    (void)args;
    (void)i;
}

// Dans la commande MSG on aura besoin de recuperer tout les clients dispo dans un channel

//  on aura besoin donc du channel et de tout les clients
// Enfaite faut toutes les infos, meme ceux du serveur, TOUT
