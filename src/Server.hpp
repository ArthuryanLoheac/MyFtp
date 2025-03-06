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
        class Client {
            public:
                Client(int client, struct sockaddr_in adr, std::string _path);
                int client;
                int dataFork = -1;
                int data;
                struct sockaddr_in adr;
                void print(std::string str);
                std::string _pathWork;

                std::string _username = "";
                bool _isUsername = false;
                std::string _password = "";
                bool _isPassword = false;
        };
        Server(std::string path, int port);
        void acceptClient();
        void closeClient(int client);
        void run();
        void readInClient(int client, int i);
    private:
        struct pollfd *getLstPoll();
        void handleCommand(std::vector<std::string> commands, int client, int i);
        bool handleCommandConnected(std::vector<std::string> commands, int i);

        void enteringPassiveMode(int client, int id);
        
        void retrFile(int id, std::string path);
        void retrXTimes(int id, std::string path);

        void commandCwd(int i, std::string path, std::string success);
        bool checkDir(std::string path, int i, std::string prev);
        void moveTo(std::string path, int i);
        void commandCdup(int i);

        void help(int i);
        void help(int i, std::string command);
        void helpCommandsRegister();

        void deleteFile(int i, std::string path);

        void userConnect(int i, std::string username);
        void passConnect(int i, std::string password);

        int serverSocket;
        struct sockaddr_in serverAddr;
        std::vector<struct pollfd> lstPoll;
        std::vector<Client> clients;
        std::string _path;

        std::map<std::string, std::string> _commandsHelp;

        int _t;
};
