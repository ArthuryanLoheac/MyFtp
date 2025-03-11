/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_CommandsConnected
*/

#include "Server.hpp"

void Server::handleCommandConnected(std::vector<std::string> commands, int i)
{
    if (commands.size() == 1 && strcmp(commands[0].c_str(), "NOOP") == 0) {
        sendResponse(fds[i].fd, "200 Command okay.\r\n");
    } else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PWD") == 0) {
        sendResponse(fds[i].fd, "257 \"" + std::get<S_PATH>(_connected[i]) + "\" created.\r\n");
    } else {
        sendResponse(fds[i].fd, "500 Unknown command.\r\n");
    }
}