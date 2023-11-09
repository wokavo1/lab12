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

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0){
      perror("[-]Socket error");
      exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while(1){
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        printf("[+]Client connected.\n");

        bzero(buffer, 1024);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("[<]Client: %s\n", buffer);
        // Записать сообщение от клиента в файл
        FILE *log = fopen("log.txt", "a+");
        if (log != NULL) {
            printf("[~]log.txt opened successfully\n");
            fputs(buffer, log);
            fputs("\n", log);
            fclose(log);
        }
        bzero(buffer, 1024);
        FILE *serverdata = fopen("serverdata.txt", "r");
        if (serverdata != NULL) {
            printf("[~]serverdata.txt opened successfully\n");
            while (fgets(buffer, 20, serverdata)) {
                printf("[>]Server: %s\n", buffer);
                send(client_sock, buffer, strlen(buffer), 0);
                bzero(buffer, 1024);
            } 
            fclose(serverdata);
        }

        close(client_sock);
        printf("[+]Client connection closed.\n\n");
    }

    return 0;
}