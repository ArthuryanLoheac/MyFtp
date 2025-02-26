/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#include "Server.hpp"
#include <string.h>

Server::Server(std::string path, int port)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr = {AF_INET, htons(port), INADDR_ANY};

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 1)
        throw std::runtime_error("Error: bind failed");
    if (listen(serverSocket, 10) == -1)
        throw std::runtime_error("Error: listen failed");
    lstPoll.push_back({serverSocket, POLLIN, 0});
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
    else
        printf("GOT: %s\n", buffer);
}

struct pollfd *Server::getLstPoll()
{
    return lstPoll.data();
}

static int getAddr()
{
    struct in_addr addr;
    struct hostent *he;

    if (inet_pton(AF_INET, "127.0.0.1", &addr) != 1) {
        char host[256];
        if (gethostname(host, sizeof(host)) == 0) {
            he = gethostbyname(host);
            if (he)
                addr = *(struct in_addr*)he->h_addr_list[0];
            else 
                throw std::runtime_error("Error: could not get local IP address");
        }
    }
    return addr.s_addr;
}

static void sendAddr(int addr, struct sockaddr_in adr, int client)
{
    int port1, port2, ip1, ip2, ip3, ip4 = 0;
    int adrresse = getAddr();
    char *buffer = new char[1024];

    port1 = ntohs(adr.sin_port) / 256;
    port2 = ntohs(adr.sin_port) % 256;
    ip1 = adrresse & 0xFF;
    ip2 = (adrresse >> 8) & 0xFF;
    ip3 = (adrresse >> 16) & 0xFF;
    ip4 = (adrresse >> 24) & 0xFF;
    sprintf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n",
        ip1, ip2, ip3, ip4, port1, port2);
    write(client, buffer, strlen(buffer));
}

void Server::enteringPassiveMode(int client, int id)
{
    clients[id].data = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {AF_INET, htons(0), INADDR_ANY};
    socklen_t len = sizeof(adr);

    if (bind(clients[id].data, (struct sockaddr *)&adr, sizeof(adr)) == 1)
        throw std::runtime_error("Error: bind failed");
    if (getsockname(clients[id].data, (struct sockaddr *)&adr, &len) == -1)
        throw std::runtime_error("Error: getsockname failed");
    sendAddr(adr.sin_addr.s_addr, adr, client);
    //DATA connection
    listen(clients[id].data, 1);
    lstPoll.push_back({clients[id].data, POLLIN, 0});
    if (accept(clients[id].data, NULL, NULL) == -1)
        throw std::runtime_error("Error: accept failed");
    printf("new data connection\n");
}

void Server::acceptClient()
{
    struct sockaddr_in adr;
    socklen_t adr_len = sizeof(adr);
    int f;

    int client = accept(serverSocket, (struct sockaddr *)&adr, &adr_len);
    if (client == -1)
        throw std::runtime_error("Error: accept failed");
    clients.push_back(Client(client, adr));
    lstPoll.push_back(clients[clients.size() - 1].pollfd);
    printf("new client : %s %d\n", inet_ntoa(adr.sin_addr), ntohs(adr.sin_port));
}

void Server::closeClient(int id)
{
    close(clients[id].client);
    clients.erase(clients.begin() + id);
    lstPoll.erase(lstPoll.begin() + id + 1);
}

void Server::run()
{
    int nbEvents;

    while(1) {
        nbEvents = poll(getLstPoll(), clients.size() + 1, -1);
        if (nbEvents == -1)
            throw std::runtime_error("Error: poll failed");
        if (lstPoll[0].revents & POLLIN)
            acceptClient();
        for (int i = 1; i < clients.size() + 1; i++) {
            if (lstPoll[i].revents & POLLIN)
                readInClient(clients[i - 1].client, i - 1);
        }
    }
}

Server::Client::Client(int client, sockaddr_in adr)
    : client(client), adr(adr)
{
    pollfd.fd = client;
    pollfd.events = POLLIN;
}