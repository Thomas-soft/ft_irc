#include "server.hpp"

Server::Server() : server_fd(-1)
{

}

Server::~Server() 
{
    if (server_fd != -1) 
        close(server_fd);
}

void Server::start(int port) 
{
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
	// Configuration du port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
	//Attachement du socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
	// Écouter sur le port
    if (listen(server_fd, 3) < 0) 
	{
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::vector<pollfd> poll_fds;
    pollfd server_pollfd = {server_fd, POLLIN, 0};
    poll_fds.push_back(server_pollfd);
    while (true) 
	{
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) 
		{
            perror("poll");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < poll_fds.size(); ++i) 
		{
            if (poll_fds[i].revents & POLLIN) 
			{
                if (poll_fds[i].fd == server_fd) 
				{
                    // Nouvelle connexion
                    int new_socket;
                    struct sockaddr_in client_address;
                    socklen_t client_len = sizeof(client_address);
                    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len)) < 0) {
                        perror("accept");
                        continue;
                    }
                    pollfd client_pollfd = {new_socket, POLLIN, 0};
                    poll_fds.push_back(client_pollfd);
                } 
				else 
				{
                    // Données reçues d'un client existant
                    char buffer[1024] = {0};
                    int valread = read(poll_fds[i].fd, buffer, 1024);
                    if (valread <= 0) 
					{
                        close(poll_fds[i].fd);
                        poll_fds.erase(poll_fds.begin() + i);
                        --i;
                    } 
					else 
					{
                        handleMessage(poll_fds[i].fd, buffer);
                    }
                }
            }
        }
    }
}

void Server::handleMessage(int client_fd, const char* message) 
{
    // Traiter le message ici
    std::cout << "Message reçu: " << message << std::endl;
    // Exemple de réponse au client
    const char* response = "Message reçu\n";
    send(client_fd, response, strlen(response), 0);
}
