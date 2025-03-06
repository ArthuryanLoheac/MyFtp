/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <sstream>

class Server
{
    public:
        class Client {
            public:
                Client(int client, struct sockaddr_in adr);
                int client;
                int dataFork = -1;
                int data;
                struct sockaddr_in adr;
                void print(std::string str);
        };
        Server(std::string path, int port);
        void acceptClient();
        void closeClient(int client);
        void run();
        void readInClient(int client, int i);
    private:
        struct pollfd *getLstPoll();
        void enteringPassiveMode(int client, int id);
        void retrFile(int id, std::string path);
        void retrXTimes(int id, std::string path);
        void handleCommand(std::vector<std::string> commands, int client, int i);

        int serverSocket;
        struct sockaddr_in serverAddr;
        std::vector<struct pollfd> lstPoll;
        std::vector<Client> clients;
        std::string _path;

        int _t;
};
