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
}

static std::vector<std::string> split(const std::string &str, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delim))
        tokens.push_back(token);
    return tokens;
}

void Server::readInClient(int client, int i)
{
    char buffer[1024];
    std::vector<std::string> commands;

    read(client, buffer, 1024);
    for (int j = 0; buffer[j]; j++)
        if (buffer[j] == '\n' || buffer[j] == '\r')
            buffer[j] = 0;
    commands = split(buffer, ' ');

    if (commands.size() == 1 && strcmp(commands[0].c_str(), "QUIT") == 0)
        closeClient(i);
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PASV") == 0)
        enteringPassiveMode(client, i);
    else if (commands.size() == 2 && strcmp(commands[0].c_str(), "RETR") == 0)
        retrFile(client, i, commands[1]);
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "OK") == 0)
        write(client, "OK\n", 3);
    else
        printf("GOT: %s\n", buffer);
    lstPoll[i].revents = 0;
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
    lstPoll.push_back({client, POLLIN, 0});
    lstPoll[0].revents = 0;
}

void Server::run()
{
    int nbEvents;

    while(1) {
        nbEvents = poll(getLstPoll(), clients.size() + 1, 1);
        if (nbEvents == -1)
            throw std::runtime_error("Error: poll failed");
        for (int i = 0; i < nbEvents; i++) {
            if (lstPoll[0].revents & POLLIN)
                acceptClient();
            for (size_t i = 1; i < clients.size() + 1; i++) {
                if (lstPoll[i].revents & POLLIN)
                    readInClient(clients[i - 1].client, i - 1);
            }
        }
    }
}

Server::Client::Client(int client, sockaddr_in adr)
    : client(client), adr(adr)
{
}