#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <pthread.h>

#include "chat.h"
#include "io/io.h"

int main(int argc, char **argv) {
    int port;
    char *ip;
    if (argc < 3 || argc > 3 || to_int(argv[2]) <= 0) {
        printf("syntax: ./client.out <ip> <port>\n");
        exit(-1);
    } else {
        port = to_int(argv[2]);
        ip = argv[1];
    }
    // Just some code to print this pretty little box
    int ip_size = strlen(ip);
    int ip_spaces = 18 - ip_size;

    int port_size = int_len(port);
    int p_spaces = abs(18 - port_size);

    clear();
    printf("# # # # # # # # # # # # # # # # # # # #\n");
    printf("#           CLIENT INITIATED          #\n");
    printf("#           PORT   %i", port);
    for (int i = 0; i < p_spaces; i++) {
        printf(" ");
    }
    printf(" #\n");
    printf("#           IP     %s", ip);
    for (int i = 0; i < ip_spaces; i++) {
        printf(" ");
    }
    printf(" #\n");
    printf("# # # # # # # # # # # # # # # # # # # #\n");

    // Raw sockets are independent of an actual protocol - might be useful later
    // This program uses TCP
    
    // Main loop bool
    int loop = 0;

    // Create a socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = INADDR_ANY;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(ip);
    // server_address.sin_addr.s_addr = INADDR_ANY;

    // Make the connection to another socket
    int connection_status = connect(
      client_socket,
      (struct sockaddr *) &server_address,
      sizeof(server_address)
    );
    
    if (connection_status == -1) {
        printf("unable to create connection to server\n");
        printf("error code: %i\n", connection_status);
        exit(-1);
    } else {
        printf("connected to the server socket\n");
        loop = 1;
    }

    // Ask the user for a username
    char *raw_username = input("username: ");
    // remove_newline(raw_username);
    // char *username = raw_username;

    // char *username = concat(raw_username, ": ");
    // printf("%s\n", username);

    // The last message that this client sent
    char **last_msg = malloc(256 * sizeof(char));

    // Assemble the arguments for the write thread
    ClientReadThreadArgs *args = malloc(sizeof(ClientReadThreadArgs *));
    args->socket = &client_socket;
    args->last_msg = last_msg;
    pthread_t id;
    pthread_create(&id, NULL, client_read_thread, args);
    // Main loop
    while (loop) {
        // Ask user for input
        char *message = input("> ");
        if (strcmp(message, "\n") != 0) {
            last_msg = &message;
            printf("====================last message: %s===========", *last_msg);
            send(client_socket, message, sizeof(message), 0);
        }
    }
    
    // Close the socket
    close(client_socket);
    return 0;
}