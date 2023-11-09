#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
    printf("argc = %d\n", argc);

    for (int i=0; i < argc; i++)
        puts(argv[i]);

    char *ip = "127.0.0.1";
    int port = 8000;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("[~]Connected to the server.\n");

    bzero(buffer, 1024);
    strcpy(buffer, "Ilya Cherkasov 8K03");
    printf("[>]Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    int accepting_file = 1;
    while(accepting_file == 1) {
        bzero(buffer, 1024);
        recv(sock, buffer, 20, 0);
        if (strcmp(buffer, "") == 0) {
            accepting_file = 0;
        } else {
            printf("[<]Server: %s\n", buffer);
        }
    }

    close(sock);
    printf("[~]Disconnected from the server.\n");

    return 0;

}