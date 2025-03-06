/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(std::string path, int port)
{
    _t = 3;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr = {AF_INET, htons(port), INADDR_ANY, 0};

    _path = path;
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 1)
        throw std::runtime_error("Error: bind failed");
    if (listen(serverSocket, 10) == -1)
        throw std::runtime_error("Error: listen failed");
    lstPoll.push_back({serverSocket, POLLIN, 0});
    helpCommandsRegister();
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

bool Server::handleCommandConnected(std::vector<std::string> commands, int i)
{
    if (commands.size() == 1 && strcmp(commands[0].c_str(), "USER") == 0) {
        userConnect(i, "anonymous");
    } else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PASS") == 0) {
        passConnect(i, "anonymous");
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "USER") == 0) {  
        userConnect(i, commands[1]);
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "PASS") == 0) {  
        passConnect(i, commands[1]);
    } else
        return false;
    return true;
}

void Server::handleCommand(std::vector<std::string> commands, int client, int i)
{
    if (commands.size() == 1 && strcmp(commands[0].c_str(), "QUIT") == 0)
        closeClient(i);
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PASV") == 0)
        enteringPassiveMode(client, i);
    else if (commands.size() == 2 && strcmp(commands[0].c_str(), "RETR") == 0)
        retrXTimes(i, commands[1]);
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "NOOP") == 0)
        clients[i].print("200 Command okay\n");
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PWD") == 0)
        clients[i].print("257 \"" + clients[i]._pathWork + "\" created.\n");
    else if (commands.size() == 2 && strcmp(commands[0].c_str(), "CWD") == 0)
        commandCwd(i, commands[1], "250 Requested file action okay, completed.\n");
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "CDUP") == 0)
        commandCwd(i, "..", "200 Command okay.\n");
    else if (commands.size() == 1 && strcmp(commands[0].c_str(), "HELP") == 0)
        help(i);
    else if (commands.size() == 2 && strcmp(commands[0].c_str(), "HELP") == 0)
        help(i, commands[1]);
    else if (commands.size() == 2 && strcmp(commands[0].c_str(), "DELE") == 0)
        deleteFile(i, commands[1]);
    else
        clients[i].print("502 Command not implemented\n");
}

void Server::readInClient(int client, int i)
{
    char buffer[1024];

    if (clients[i].dataFork != 0 && clients[i].dataFork != -1
        && read(clients[i].dataFork, buffer, 0) < 0) {
        if (kill(clients[i].dataFork, 0) != 0)
            return;
    }
    read(client, buffer, 1024);
    for (int j = 0; buffer[j]; j++)
        if (buffer[j] == '\n' || buffer[j] == '\r')
            buffer[j] = 0;
    if (handleCommandConnected(split(buffer, ' '), i) == false) {
        if (clients[i]._isUsername == false || clients[i]._isPassword == false)
            clients[i].print("530 Please login with USER and PASS.\n");
        else
            handleCommand(split(buffer, ' '), client, i);
    }
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
    clients.push_back(Client(client, adr, _path));
    lstPoll.push_back({client, POLLIN, 0});
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
                if (lstPoll[i].revents & POLLIN){
                    readInClient(clients[i - 1].client, i - 1);
                }
            }
        }
    }
}

Server::Client::Client(int client, sockaddr_in adr, std::string _path)
    : client(client), adr(adr)
{
    _pathWork = _path;
}

void Server::Client::print(std::string str)
{
    write(client, str.c_str(), str.size());
}
