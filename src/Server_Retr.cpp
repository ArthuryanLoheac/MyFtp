/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Retr
*/

#include "Server.hpp"

static void SendFileInSocket(std::string path, std::string _path, int fd)
{
    std::string fullPath = _path;
    char buffer[1024];
    int file;
    int size;

    if (fullPath[fullPath.size() - 1] != '/')
        fullPath += "/";
    fullPath += path;
    file = open(fullPath.c_str(), O_RDONLY);
    if (file == -1)
        throw std::runtime_error("Error: could not open file : " + fullPath);
    size = read(file, buffer, 1024);
    while (size > 0) {
        write(fd, buffer, size);
        size = read(file, buffer, 1024);
    }
}

void Server::retrFile(int id, std::string path)
{
    if (clients[id].dataFork == 0) {
        SendFileInSocket(path, _path, clients[id].data);
        close(clients[id].data);
        clients[id].dataFork = -1;
        exit(0);
    }
}