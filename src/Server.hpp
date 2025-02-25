/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

class Server
{
    public:
        class socketClient
        {
            public:
                struct pollfd socket;
                struct pollfd dataSocket;
        };

        Server(std::string path, int port);
        std::vector<struct pollfd> getSockets();
        void run();
    protected:
    private:
        void eventNewClient();

        std::vector<socketClient> clients;
        std::string path;
        int port;
        struct sockaddr_in server;
        struct pollfd fdServer;
};
