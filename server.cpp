#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket)
{
    char buffer[BUFSIZ];
    char msg_to_send[BUFSIZ];
    int bytes_read;
    int status;

    memset(&buffer, '\0', sizeof buffer);
    bytes_read = recv(socket, buffer, BUFSIZ, 0);
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            printf("[%d] Client socket closed connection.\n", socket);
        }
        else {
            fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
        }
        close(socket); // Ferme la socket
        FD_CLR(socket, all_sockets); // Enlève la socket de l'ensemble
    }
    else {
        // Renvoie le message reçu à toutes les sockets connectées
        // à part celle du serveur et celle qui l'a envoyée
        printf("[%d] Got message: %s", socket, buffer);
        memset(&msg_to_send, '\0', sizeof msg_to_send);
        printf(msg_to_send, "[%d] says: %s", socket, buffer);
        for (int j = 0; j <= fd_max; j++) {
            if (FD_ISSET(j, all_sockets) && j != server_socket && j != socket) {
                status = send(j, msg_to_send, strlen(msg_to_send), 0);
                if (status == -1) {
                    fprintf(stderr, "[Server] Send error to client fd %d: %s\n", j, strerror(errno));
                }
            }
        }
    }
}

void accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max)
{
    int client_fd;
    char msg_to_send[BUFSIZ];
    int status;

    client_fd = accept(server_socket, NULL, NULL);
    if (client_fd == -1) {
        fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
        return ;
    }
    FD_SET(client_fd, all_sockets); // Ajoute la socket client à l'ensemble
    if (client_fd > *fd_max) {
        *fd_max = client_fd; // Met à jour la plus grande socket
    }
    printf("[Server] Accepted new connection on client socket %d.\n", client_fd);
    memset(&msg_to_send, '\0', sizeof msg_to_send);
    sprintf(msg_to_send, "Welcome. You are client fd [%d]\n", client_fd);
    status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
    if (status == -1) {
        fprintf(stderr, "[Server] Send error to client %d: %s\n", client_fd, strerror(errno));
    }
}

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(server, F_SETFL, O_NONBLOCK);
    char buf[1024];
    sockaddr_in servInfo;

    fd_set all_sockets;
    fd_set read_fds;
    int fd_max;
    struct timeval timer;

    servInfo.sin_family = AF_INET;
    servInfo.sin_port = htons(8080);
    servInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    bind(server, (struct sockaddr*)&servInfo, sizeof(servInfo));
    int status = listen(server, 20);

    FD_ZERO(&all_sockets);
    FD_ZERO(&read_fds);
    FD_SET(server, &all_sockets);
    fd_max = server;
    std::cout << "[Server] set up select fd sets \n" << std::endl;

    while(1)
    {
        read_fds = all_sockets;
        
        timer.tv_sec = 2;
        timer.tv_usec = 0;

        status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
        if (status == -1)
        {
            fprintf(stderr, "[Server] select error: %s\n", strerror(errno));
            exit(1);
        }
        else if (status == 0)
        {
            std::cout << "[Server] Waiting.... " << std::endl;
            continue;
        }

        for (int i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_fds) != 1)
            {
                continue;
            }
            std::cout << "[Serveur] Ready for I/O operation" << std::endl;
            if (i == server)
            {
                accept_new_connection(server, &all_sockets, &fd_max);
            }
            else
            {
                read_data_from_socket(i, &all_sockets, fd_max, server);
            }
        }
    }
    return 0;

    // int client_fd = accept(server, NULL, NULL);
    // recv(client_fd, buf, sizeof(buf), 0);
    // std::cout << buf << std::endl;
    // // std::cout << "Message client bien reçu" << std::endl;
}