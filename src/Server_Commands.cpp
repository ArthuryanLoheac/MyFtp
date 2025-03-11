/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Commands
*/

#include "Server.hpp"

void Server::handleCommand(std::string command, int i)
{
    if (strcmp(command.c_str(), "QUIT") == 0) {
        dprintf(fds[i].fd, "221 Service closing control connection.\r\n");
        closeClient(i);
    } else
        dprintf(fds[i].fd, "500 Unknown command.\r\n");
}