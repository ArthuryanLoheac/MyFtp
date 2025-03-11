/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Commands
*/

#include "Server.hpp"

static void sendResponse(int fd, std::string response)
{
    write(fd, response.c_str(), response.size());
}

void Server::handleCommand(std::string command, int i)
{
    if (strcmp(command.c_str(), "QUIT") == 0) {
        sendResponse(fds[i].fd, "221 Service closing control connection.\r\n");
        closeClient(i);
    } else if (strcmp(command.c_str(), "NOOP") == 0) {
        sendResponse(fds[i].fd, "200 Command okay.\r\n");
    } else {
        sendResponse(fds[i].fd, "500 Unknown command.\r\n");
    }
}