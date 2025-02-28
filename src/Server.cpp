/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(std::string path, int port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr = {AF_INET, htons(port), INADDR_ANY, 0};

    _path = path;
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 1)
        throw std::runtime_error("Error: bind failed");
    if (listen(serverSocket, 10) == -1)
        throw std::runtime_error("Error: listen failed");
    lstPoll.push_back({serverSocket, POLLIN, 0});
    lstIsData.push_back(false);
}

void Server::acceptInData(int client, int i)
{
    accept(client, NULL, NULL);
    clients[i].pollfd.revents = 0;
}

void Server::readInClient(int client, int i)
{
    char buffer[1024];

    read(client, buffer, 1024);
    for (int j = 0; buffer[j]; j++)
        if (buffer[j] == '\n' || buffer[j] == '\r')
            buffer[j] = 0;

    if (strcmp(buffer, "QUIT") == 0)
        closeClient(i);
    else if (strcmp(buffer, "PASV") == 0)
        enteringPassiveMode(client, i);
    else if (strcmp(buffer, "RETR") == 0)
        enteringPassiveMode(client, i);
    else
        printf("GOT: %s\n", buffer);
    clients[i].pollfd.revents = 0;
}

struct pollfd *Server::getLstPoll()
{
    return lstPoll.data();
}

void Server::acceptClient()
{
    struct sockaddr_in adr;
    socklen_t adr_len = sizeof(adr);

    int client = accept(serverSocket, (struct sockaddr *)&adr, &adr_len);
    if (client == -1)
        throw std::runtime_error("Error: accept failed");
    clients.push_back(Client(client, adr));
    lstPoll.push_back(clients[clients.size() - 1].pollfd);
    lstIsData.push_back(false);
    lstPoll[0].revents = 0;
}

void Server::run()
{
    int nbEvents;

    while(1) {
        nbEvents = poll(getLstPoll(), clients.size() + 1, -1);
        if (nbEvents == -1)
            throw std::runtime_error("Error: poll failed");

        for (int i = 0; i < nbEvents; i++) {
            if (lstPoll[0].revents & POLLIN)
                acceptClient();
            for (size_t i = 1; i < clients.size() + 1; i++) {
                if (lstPoll[i].revents & POLLIN) {
                    if (lstIsData[i])
                        acceptInData(clients[i - 1].data, i - 1);
                    else
                        readInClient(clients[i - 1].client, i - 1);
                }
            }
        }
    }
}

Server::Client::Client(int client, sockaddr_in adr)
    : client(client), adr(adr)
{
    pollfd.fd = client;
    pollfd.events = POLLIN;
}