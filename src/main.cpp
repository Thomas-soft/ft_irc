#include <iostream>
#include "../includes/Server.hpp"

int ft_atoi(char *cPort)
{
    std::string port = cPort;

    // Verifier si il contient que des chiffres
    for (size_t i = 0; i < port.size(); i++)
        if (!std::isdigit(port[i]))
            return (-1);
    return (std::atoi(cPort));
}

int main(int ac, char **av)
{
	int				port;
	std::string		pass;

	if (ac != 3)
	{
		std::cerr << "Wrong number of arguments !" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "./ft_ircserv <port> <password>" << std::endl;
		return (1);
	}
	port = ft_atoi(av[1]);
	pass = av[2];
	if (port == -1 || port < 0 || port > 65335)
	{
		std::cerr << "Error with port number !" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "./ft_ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}
    Server  server(port, pass);
    server.start();
    return (EXIT_SUCCESS);
}