/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Commands
*/

#include "Server.hpp"

void Server::sendResponse(int fd, std::string response)
{
    write(fd, response.c_str(), response.size());
}

void Server::handleCommand(std::vector<std::string> commands, int i)
{
    if (commands.size() == 1 && strcmp(commands[0].c_str(), "QUIT") == 0) {
        sendResponse(fds[i].fd, "221 Service closing control connection.\r\n");
        closeClient(i);
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "USER") == 0) {
        command_user(commands[1], i);
    } else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PASS") == 0) {
        command_pass("", i);
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "PASS") == 0) {
        command_pass(commands[1], i);
    } else if (std::get<0>(_connected[i]) && std::get<1>(_connected[i])) {
        handleCommandConnected(commands, i);
    } else {
        sendResponse(fds[i].fd, "530 Not logged in.\r\n");
    }
}

// ================== COMMANDS ==================

void Server::command_user(std::string user, int i)
{
    if (std::get<B_USER>(_connected[i]) == true) {
        sendResponse(fds[i].fd, "550 Already connected.\r\n");
        return;
    } else {
        std::get<B_USER>(_connected[i]) = true;
        std::get<S_USER>(_connected[i]) = user;
        sendResponse(fds[i].fd, "331 User name okay, need password.\r\n");
    }
}

void Server::command_pass(std::string pass, int i)
{
    if (std::get<B_PASS>(_connected[i]) == true) {
        sendResponse(fds[i].fd, "550 Already connected.\r\n");
        return;
    }
    if (std::get<B_USER>(_connected[i]) == false) {
        sendResponse(fds[i].fd, "503 Bad sequence of commands.\r\n");
        return;
    }
    std::get<B_PASS>(_connected[i]) = true;
    std::get<S_PASS>(_connected[i]) = pass;
    sendResponse(fds[i].fd, "230 User logged in, proceed.\r\n");
}
