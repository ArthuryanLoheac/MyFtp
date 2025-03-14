/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_CommandsConnected
*/

#include "Server.hpp"

void Server::commandCwd(std::string path, int i, std::string message)
{
    std::string prev_path = std::get<S_PATH>(_connected[i]);
    std::string new_path;
    struct stat buffer;

    if (path.size() == 0) {
        sendResponse(fds[i].fd, "550 Failed to change directory.\r\n");
        return;
    }
    if (std::get<S_PATH>(_connected[i])[std::get<S_PATH>(_connected[i]).size() - 1] == '/')
        new_path = std::get<S_PATH>(_connected[i]) + path;
    else
        new_path = std::get<S_PATH>(_connected[i]) + "/" + path;
    if (stat(new_path.c_str(), &buffer) == 0) {
        if (strcmp(path.c_str(), "..") == 0) {
            size_t pos = std::get<S_PATH>(_connected[i]).find_last_of("/");
            if (pos == std::string::npos) {
                sendResponse(fds[i].fd, "550 Failed to change directory.\r\n");
                return;
            }
            prev_path = new_path;
            new_path = std::get<S_PATH>(_connected[i]).substr(0, pos);
        } else
            new_path = std::get<S_PATH>(_connected[i]) + "/" + path;
        std::get<S_PATH>(_connected[i]) = new_path;
        sendResponse(fds[i].fd, message);
        return;
    }
    sendResponse(fds[i].fd, "550 Failed to change directory.\r\n");
}

void Server::commandDele(std::string path, int i)
{
    std::string new_path;
    struct stat buffer;

    if (path.size() == 0) {
        sendResponse(fds[i].fd, "550 Failed to delete file.\r\n");
        return;
    }
    if (std::get<S_PATH>(_connected[i])[std::get<S_PATH>(_connected[i]).size() - 1] == '/')
        new_path = std::get<S_PATH>(_connected[i]) + path;
    else
        new_path = std::get<S_PATH>(_connected[i]) + "/" + path;
    if (stat(new_path.c_str(), &buffer) == 0) {
        if (remove(new_path.c_str()) == 0) {
            sendResponse(fds[i].fd, "250 Requested file action okay, completed.\r\n");
            return;
        }
    }
    sendResponse(fds[i].fd, "550 Failed to delete file.\r\n");
}

void Server::commandHelp(int i)
{
    sendResponse(fds[i].fd, "214 The following commands are recognized.\r\n");
    sendResponse(fds[i].fd, "USER PASS PWD CWD CDUP DELE NOOP HELP QUIT\r\n");
    sendResponse(fds[i].fd, "214 Help OK.\r\n");
}

void Server::commandHelp(std::string path, int i)
{
    try {
        std::string help = _commands.at(path);
        sendResponse(fds[i].fd, "214 " + help);
    } catch (const std::out_of_range &e) {
        sendResponse(fds[i].fd, "500 Unknown command.\r\n");
    }
}

void Server::handleCommandConnected(std::vector<std::string> commands, int i)
{
    if (commands.size() == 1 && strcmp(commands[0].c_str(), "NOOP") == 0) {
        sendResponse(fds[i].fd, "200 Command okay.\r\n");
    } else if (commands.size() == 1 && strcmp(commands[0].c_str(), "PWD") == 0) {
        sendResponse(fds[i].fd, "257 \"" + std::get<S_PATH>(_connected[i]) + "\" created.\r\n");
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "CWD") == 0) {
        commandCwd(commands[1], i, "250 Requested file action okay, completed.\r\n");
    } else if (commands.size() == 1 && strcmp(commands[0].c_str(), "CDUP") == 0) {
        commandCwd("..", i, "200 Command okay.\r\n");
    } else if (commands.size() == 2 && strcmp(commands[0].c_str(), "DELE") == 0) {
        commandDele(commands[1], i);
    } else {
        sendResponse(fds[i].fd, "500 Unknown command.\r\n");
    }
}