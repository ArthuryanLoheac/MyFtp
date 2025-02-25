/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server
*/

#pragma once
#include <string>

class Server
{
    public:
        class socketClient
        {
            public:
                int getSocket() { return socket; }
                int getDataSocket() { return dataSocket; }
                void setSocket(int socket) { this->socket = socket; }
                void setDataSocket(int dataSocket) { this->dataSocket = dataSocket; }
            protected:
            private:
                int socket;
                int dataSocket;
        };
    protected:
    private:
        std::string path;
        int port;
        int socket;
};
