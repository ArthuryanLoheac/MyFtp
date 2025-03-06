/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Retr
*/

#include "Server.hpp"

static int SendFileInSocket(std::string path, std::string _path, int fd)
{
    std::string fullPath = _path;
    char buffer[1024];
    int file;
    int size;

    if (fullPath[fullPath.size() - 1] != '/')
        fullPath += "/";
    fullPath += path;
    file = open(fullPath.c_str(), O_RDONLY);
    if (file == -1){
        write(fd, "550 Requested action not taken. File unavailable.\n", 49);
        return 1;
    }
    size = read(file, buffer, 1024);
    while (size > 0) {
        write(fd, buffer, size);
        size = read(file, buffer, 1024);
    }
    close(file);
    return 0;
}

void Server::retrFile(int id, std::string path)
{
    if (clients[id].dataFork == 0) {
        write(clients[id].data, "150 File status okay; about to open data connection.\n", 52);
        if (SendFileInSocket(path, clients[id]._pathWork, clients[id].data) == 0)
            write(clients[id].client, "226 Closing data connection. Requested file action successful.\n", 63);
        else
            write(clients[id].client, "550 Requested action not taken. File unavailable.\n", 49);
        close(clients[id].data);
        clients[id].dataFork = -1;
        exit(0);
    } else
        clients[id].dataFork = -1;
}

void Server::retrXTimes(int id, std::string path)
{
    for (int i = 0; i < _t; i++)
        retrFile(id, path);
    _t = 1;
}
