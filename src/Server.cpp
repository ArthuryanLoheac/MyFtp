/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(std::string path, int port)
{
    _path = path;
    _port = port;
    ServerInit();
}

void Server::ServerInit()
{
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw std::runtime_error("Socket creation failed");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Bind failed");
    if (listen(server_fd, 3) < 0)
        throw std::runtime_error("Listen failed");
    fds.push_back({server_fd, POLLIN, 0});
}

void Server::ServerAccept()
{
    if (fds[0].revents & POLLIN) {
        int new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket < 0)
            throw std::runtime_error("Accept failed");
        fds.push_back({new_socket, POLLIN, 0});
    }
}

void Server::run()
{
    while (true) {
        if (poll(fds.data(), fds.size(), -1) < 0)
            throw std::runtime_error("Poll failed");
        ServerAccept();
        for (size_t i = 1; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN)
                readClient(i);
        }
    }
}

void Server::readClient(size_t &i)
{
    char buffer[1024] = {0};
    int valread = read(fds[i].fd, buffer, 1024);
    if (valread <= 0) {
        closeClient(i);
        i--;
        return;
    }
    handleCommand(buffer, i);
}

void Server::closeClient(int i)
{
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
}

