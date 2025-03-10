/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Quit
*/

#include "Server.hpp"
#include <signal.h>

void Server::closeClient(int id)
{
    write(clients[id].client, "221 Service closing control connection.\n", 40);
    close(clients[id].client);
    close(clients[id].dataFork);
    close(clients[id].data);
    clients.erase(clients.begin() + id);
    lstPoll.erase(lstPoll.begin() + id + 1);
    kill(clients[id].client, SIGKILL);
}