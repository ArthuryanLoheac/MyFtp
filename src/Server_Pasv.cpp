/*
** EPITECH PROJECT, 2025
** MyFtp
** File description:
** Server_Pasv
*/

#include "Server.hpp"

static int getAddr()
{
    struct in_addr addr;
    struct hostent *he;

    if (inet_pton(AF_INET, "127.0.0.1", &addr) != 1) {
        char host[256];
        if (gethostname(host, sizeof(host)) == 0) {
            he = gethostbyname(host);
            if (he)
                addr = *(struct in_addr*)he->h_addr_list[0];
            else 
                throw std::runtime_error("Error: could not get local IP address");
        }
    }
    return addr.s_addr;
}

static void sendAddr(struct sockaddr_in adr, int client)
{
    int port1, port2, ip1, ip2, ip3, ip4 = 0;
    int adrresse = getAddr();
    char *buffer = new char[1024];

    port1 = ntohs(adr.sin_port) / 256;
    port2 = ntohs(adr.sin_port) % 256;
    ip1 = adrresse & 0xFF;
    ip2 = (adrresse >> 8) & 0xFF;
    ip3 = (adrresse >> 16) & 0xFF;
    ip4 = (adrresse >> 24) & 0xFF;
    sprintf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n",
        ip1, ip2, ip3, ip4, port1, port2);
    write(client, buffer, strlen(buffer));
}

void Server::enteringPassiveMode(int client, int id)
{
    id = id;
    int *a = new int;
    *a = 0;
    int data = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {AF_INET, htons(0), INADDR_ANY, 0};
    socklen_t len = sizeof(adr);

    if (bind(data, (struct sockaddr *)&adr, sizeof(adr)) == -1)
        throw std::runtime_error("Error: bind failed");
    if (getsockname(data, (struct sockaddr *)&adr, &len) == -1)
        throw std::runtime_error("Error: getsockname failed");
    sendAddr(adr, client);
    //DATA connection
    listen(data, 1);
    int f = fork();
    if (f == 0) {
        printf("Data client waiting\n");
        int dataClient = accept(data, NULL, NULL);
        if (dataClient == -1) {
            perror("Error: accept failed");
            exit(1);
        }
        printf("Data client accepted\n");
        *a = 2;
        while (1);
        close(dataClient);
        exit(0);
    } else {
        sleep(20);
        printf("%d\n", *a);
    }
}
