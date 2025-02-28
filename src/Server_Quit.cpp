/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Quit
*/

#include "Server.hpp"

void Server::closeClient(int id)
{
    close(clients[id].client);
    clients.erase(clients.begin() + id);
    lstPoll.erase(lstPoll.begin() + id + 1);
}