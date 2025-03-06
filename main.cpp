/*
** EPITECH PROJECT, 2025
** Test
** File description:
** main
*/

#include <iostream>
#include <string.h>
#include "Server.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int startServer(int port, std::string path)
{
    Server server(path, port);
    server.run();
    return 0;
}

int help()
{
    printf("USAGE: ./myftp port path\n"
        "\tport is the port number on which the server socket listens\n"
        "\tpath is the path to the home directory for the Anonymous user\n");
    return 0;
}

static int returnError(std::string message)
{
    std::cerr << message << std::endl;
    return 84;
}

int checkArgs(std::string port, std::string path)
{
    char *endPtr;
    int portInt = strtol(port.c_str(), &endPtr, 10);
    struct stat pathStat;

    for (auto c : port)
        if (c < '0' || c > '9')
            return returnError("Error: port is not a number");
    if (endPtr < port.c_str() + port.size()) 
        return returnError("Error: port is not a number");
    if (path != "." && access(path.c_str(), F_OK) == -1)
        return returnError("Error: path does not exist");
    if (stat(path.c_str(), &pathStat) != 0 || !S_ISDIR(pathStat.st_mode))
        return returnError("Error: path is not a directory");
    return startServer(portInt, path);
}

int main (int ac, char **av)
{
    std::string port;
    std::string path;

    if (ac == 2 && (strcmp(av[1], "-help") == 0 || strcmp(av[1], "-h") == 0))
        return help();
    if (ac == 3) {
        port = av[1];
        path = av[2];
        return checkArgs(port, path);
    }
    return 84;
}
