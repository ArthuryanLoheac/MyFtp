/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#include "Server.hpp"

static int return_error(const std::string &message)
{
    std::cerr << message << std::endl;
    return 84;
}

Server::Server(std::string path, int port)
{
    fdServer.fd = socket(AF_INET, SOCK_STREAM, 0);
    fdServer.events = POLLIN;

    server = {AF_INET, htons(port), INADDR_ANY};
    if (fdServer.fd == -1)
        throw std::runtime_error("Error: socket creation failed");
    if (bind(fdServer.fd, (struct sockaddr *)&server, sizeof(server)) == -1)
        throw std::runtime_error("Error: bind failed");
    if (listen(fdServer.fd, 10) == -1)
        throw std::runtime_error("Error: listen failed");
}

std::vector<struct pollfd> Server::getSockets()
{
    std::vector<struct pollfd> fds;

    fds.push_back(fdServer);
    for(auto client : clients)
        fds.push_back(client.socket);
    return fds;
}

void Server::eventNewClient()
{
    struct sockaddr_in client;
    socklen_t client_len;
    socketClient Client;

    std::cout << "WAIT client" << std::endl;
    if (fdServer.revents & POLLIN){
        std::cout << "RECEIVE client" << std::endl;
        Client.socket.events = POLLIN | POLLOUT;
        Client.socket.fd = accept(fdServer.fd, (struct sockaddr *) &client, &client_len);
        if (Client.socket.fd == -1)
            throw std::runtime_error("Error: accept failed");
        std::cout << "New client connected" << std::endl;
        clients.push_back(Client);    
    }
}

void Server::run()
{
    int nbEvents;

    while (true) {
        nbEvents = poll(getSockets().data(), getSockets().size(), -1);
        if (nbEvents < 0)
            throw std::runtime_error("Error: poll failed");
        eventNewClient();
        //handleEvents(fds);
    }
}
