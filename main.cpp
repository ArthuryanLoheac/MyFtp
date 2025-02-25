/*
** EPITECH PROJECT, 2025
** Test
** File description:
** main
*/

#include <iostream>
#include <string.h>
#include "Server.hpp"

int startServer(int port, std::string path)
{
    
    return 0;
}

int help()
{
    printf("USAGE: ./myftp port path\n"
        "\tport is the port number on which the server socket listens\n"
        "\tpath is the path to the home directory for the Anonymous user\n");
    return 0;
}

int checkArgs(std::string port, std::string path)
{
    char *endPtr;
    int portInt = strtol(port.c_str(), &endPtr, 10);

    for (auto c : port)
        if (c < '0' || c > '9')
            return 84;
    if (endPtr < port.c_str() + port.size())
        return 84;
    return 0;
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
