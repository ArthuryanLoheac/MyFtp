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
#include <map>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

class Server
{
    public:
        Server(std::string path, int port);
        void ServerInit();
        void ServerAccept();
        void run();
        void readClient(int i);
        void closeClient(int i);
    
    private:
        std::string _path;
        int _port;
        std::vector<pollfd> fds;
        int server_fd;
};
