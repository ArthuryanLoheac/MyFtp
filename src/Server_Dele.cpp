/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Dele
*/
#include "Server.hpp"

void Server::deleteFile(int i, std::string path)
{
    std::string pathEdited = clients[i]._pathWork + "/" + path;

    if (remove(pathEdited.c_str()) != 0)
        clients[i].print("550 Requested action not taken.\n");
    else
        clients[i].print("250 Requested file action okay, completed.\n");
}
