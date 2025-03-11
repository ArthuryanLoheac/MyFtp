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
        void readClient(size_t &i);
        void closeClient(int i);
        void handleCommand(std::vector<std::string> commands, int i);
        void handleCommandConnected(std::vector<std::string> commands, int i);
        void sendResponse(int fd, std::string response);

        void command_user(std::string user, int i);
        void command_pass(std::string pass, int i);
        void commandCwd(std::string path, int i, std::string message);
    
    private:
        enum ConnectedType {
            B_USER = 0,
            B_PASS = 1,
            S_USER = 2,
            S_PASS = 3,
            S_PATH = 4
        };
        std::string _path;
        int _port;
        std::vector<pollfd> fds;
        std::vector<std::tuple<bool, bool, std::string, std::string, std::string>> _connected; 
            // bool = user, bool = pass, string = user, string = pass, string = path
        int server_fd;
};
