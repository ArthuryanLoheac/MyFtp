/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Cwd
*/

#include "Server.hpp"

static bool checkDirectory(std::string path)
{
    struct stat pathStat;
 
    if (path != "." && access(path.c_str(), F_OK) == -1)
        return false;
    if (stat(path.c_str(), &pathStat) != 0 || !S_ISDIR(pathStat.st_mode))
        return false;
    return true;
}

static std::vector<std::string> split(const std::string &str, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delim))
        tokens.push_back(token);
    return tokens;
}

void Server::commandCwd(int i, std::string path, std::string success)
{
    std::vector<std::string> pathSplit = split(path, '/');
    std::string prev = clients[i]._pathWork;

    for (size_t j = 0; j < pathSplit.size(); j++) {
        prev = clients[i]._pathWork;
        if (strcmp(pathSplit[j].c_str(), "..") == 0) {
            if (strcmp(_path.c_str(), clients[i]._pathWork.c_str()) == 0) {
                clients[i].print("550 Can't go above parent directory.\n");
                return;
            } else
                commandCdup(i);
        } else if (strcmp(pathSplit[j].c_str(), ".") != 0)
            moveTo(pathSplit[j], i);
        if (!checkDir(clients[i]._pathWork, i, prev))
            return;
    }
    clients[i].print(success);
}

bool Server::checkDir(std::string path, int i, std::string prev)
{
    if (!checkDirectory(clients[i]._pathWork)) {
        clients[i]._pathWork = prev;
        clients[i].print("550 Can't change directory to " + path + "\n");
        return false;
    }
    return true;
}

void Server::moveTo(std::string path, int i)
{
    if (clients[i]._pathWork[clients[i]._pathWork.size() - 1] != '/')
        clients[i]._pathWork += "/";
    clients[i]._pathWork += path;
}

void Server::commandCdup(int i)
{
    clients[i]._pathWork = clients[i]._pathWork.substr(0, clients[i]._pathWork.size() - 1);
    while (clients[i]._pathWork[clients[i]._pathWork.size() - 1] != '/')
        clients[i]._pathWork = clients[i]._pathWork.substr(0, clients[i]._pathWork.size() - 1);
    clients[i]._pathWork = clients[i]._pathWork.substr(0, clients[i]._pathWork.size() - 1);
}
