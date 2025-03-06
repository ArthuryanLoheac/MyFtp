/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Help
*/

#include "Server.hpp"

void Server::helpCommandsRegister()
{
    _commandsHelp["USER"] = "USER <SP> <username> <CRLF>";
    _commandsHelp["PASS"] = "PASS <SP> <password> <CRLF>";
    _commandsHelp["CWD"] = "CWD <SP> <pathname> <CRLF>";
    _commandsHelp["CDUP"] = "CDUP <CRLF>";
    _commandsHelp["QUIT"] = "QUIT <CRLF>";
    _commandsHelp["DELE"] = "DELE <SP> <pathname> <CRLF>";
    _commandsHelp["PWD"] = "PWD <CRLF>";
    _commandsHelp["PASV"] = "PASV <CRLF>";
    _commandsHelp["PORT"] = "PORT <SP> <host-port> <CRLF>";
    _commandsHelp["HELP"] = "HELP [<SP> <string>] <CRLF>";
    _commandsHelp["NOOP"] = "NOOP <CRLF>";
    _commandsHelp["RETR"] = "RETR <SP> <pathname> <CRLF>";
    _commandsHelp["LIST"] = "LIST [<SP> <pathname>] <CRLF>";
    _commandsHelp["STOR"] = "STOR <SP> <pathname> <CRLF>";
}
void Server::help(int i)
{
    clients[i].print("214-The following commands are recognized.\n");
    clients[i].print("USER PASS CWD CDUP QUIT DELE PWD PASV PORT HELP NOOP RETR LIST STOR\n");
    clients[i].print("214 Help OK.\n");
}

void Server::help(int i, std::string command)
{
    if (_commandsHelp.find(command) == _commandsHelp.end()){
        clients[i].print("502 Command not implemented\n");
        return;
    }
    clients[i].print("214 " + _commandsHelp.at(command) + "\n");
}
