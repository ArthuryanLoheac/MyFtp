/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Commands
*/

#include "Server.hpp"

void Server::handleCommand(std::string command, int i)
{
    for (size_t j = 0; j < command.size(); j++)
        if (command[j] == '\r' || command[j] == '\n')
            command[j] = '\0';
    if (strcmp(command.c_str(), "QUIT") == 0)
        closeClient(i);
    else
        dprintf(fds[i].fd, "500 Unknown command.\r\n");
}