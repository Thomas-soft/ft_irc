#include "ft_irc.hpp"

int ft_atoi(char *str)
{
	int nbr;
	int i;

	i = 0;
	nbr = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
	}
	if (nbr > 65535)
		return (-1);
	return (nbr);
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
	if (port == -1)
	{
		std::cerr << "Error with port number !" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "./ft_ircserv <port> <password>" << std::endl;
		return (1);
	}
	std::cout << port << " " << pass << std::endl;
}