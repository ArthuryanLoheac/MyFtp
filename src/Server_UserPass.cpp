/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_UserPass
*/

#include "Server.hpp"

void Server::userConnect(int i, std::string username)
{
    clients[i]._username = username;
    clients[i]._isUsername = true;
    if (clients[i]._isPassword == false)
        clients[i].print("331 User name okay, need password.\n");
    else
        clients[i].print("230 User logged in, proceed.\n");
}

void Server::passConnect(int i, std::string password)
{
    clients[i]._password = password;
    clients[i]._isPassword = true;
    if (clients[i]._isUsername == false)
        clients[i].print("332 Need account for login.\n");
    else
        clients[i].print("230 User logged in, proceed.\n");
}
