#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>



/**
 *                      IPK Project 1: IPK Calculator Protocol
 *                          Andrej Horacek - xhorac20
 * */

#define BUFFER_SIZE 1024

// Deklaracie funkcii
/**
 * siglnal - prijaty signal od uzivatela
 * */
void handle_interrupt(int signal);

/**
 * port - cislo portu zadane na vstup
 * host - adresa zadana na vstupe
 * */
void tcp_client(const char *host, int port);

/**
 * port - cislo portu zadane na vstup
 * host - adresa zadana na vstupe
 * */
void udp_client(const char *host, int port);

/**
 * argc - pocet argumentov, zadanych do prikazoveho riadku.
 * argv - pole retazcov obsahujuce jednotlive argumenty
 * */
int main(int argc, char *argv[]) {
    // Kontrola poctu argumentov
    if (argc != 7) {
        fprintf(stderr, "Usage: %s -h <host> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Inicializacia premennych host, port, režim
    const char *host, *mode = NULL;
    int port = 0;

    // Kontrola platnosti zadaných argumentov
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-h") == 0) {
            host = argv[i + 1];
        } else if (strcmp(argv[i], "-p") == 0) {
            port = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-m") == 0) {
            mode = argv[i + 1];
        }
    }

    // Kontrola validity argumentov
    if (host == NULL || port <= 0 || mode == NULL) {
        fprintf(stderr, "Invalid arguments. Usage: %s -h <host> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Spracovanie (Ctrl+C)
    signal(SIGINT, handle_interrupt);

    // Volanie funkcii podla zvoleneho rezimu
    if (strcmp(mode, "tcp") == 0) {
        tcp_client(host, port);
    } else if (strcmp(mode, "udp") == 0) {
        udp_client(host, port);
    } else {
        fprintf(stderr, "Invalid mode. Use either 'tcp' or 'udp'.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}


// UDP client
void udp_client(const char *host, int port) {
    // Vytvorenie UDP socketu
    int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_udp < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Konfiguracia serverovej adresy
    struct sockaddr_in server_addres;
    memset(&server_addres, 0, sizeof(server_addres));
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addres.sin_addr);

    socklen_t server_addr_len = sizeof(server_addres);

    char buffer[BUFFER_SIZE];

    // Citanie zo standratneho vstupu
    while (fgets(buffer, sizeof(buffer), stdin)) {
        // Priprava spravy
        uint8_t r_opcode = 0;
        uint8_t length = strlen(buffer);
        uint8_t message[length + 2];

        message[0] = r_opcode;
        message[1] = length;
        memcpy(message + 2, buffer, length);

        // Odoslanie spravy
        sendto(socket_udp, message, sizeof(message), 0, (struct sockaddr *) &server_addres, server_addr_len);

        // Prijatie spravy
        uint8_t response[3 + BUFFER_SIZE];
        recvfrom(socket_udp, response, sizeof(response), 0, (struct sockaddr *) &server_addres, &server_addr_len);

        // Parsovanie prijatej spravy
        r_opcode = response[0];
        uint8_t status_code = response[1];
        length = response[2];

        // Vypisanie prijatej spravy
        if (r_opcode == 1 && status_code == 0) {
            printf("OK:%.*s\n", length, response + 3);
        } else if (r_opcode == 1 && status_code == 1) {
            fprintf(stderr, "ERR:%.*s", length, response + 3);
        } else {
            fprintf(stderr, "Error: Invalid server response");
        }
    }

    // ukoncenie soketu
    close(socket_udp);
}

// Definicia funkcie pre spracovanie CTRL+C
void handle_interrupt(int signal) {
    if (signal == SIGINT) {
        fprintf(stderr, "Interrupt signal received. Exiting...\n");
        exit(EXIT_SUCCESS);
    }
}

// TCP client
void tcp_client(const char *host, int port) {
    // Vytvorenie TCP socketu
    int socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_tcp < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Konfiguracia serverovej adresy
    struct sockaddr_in server_addres;
    memset(&server_addres, 0, sizeof(server_addres));
    server_addres.sin_family = AF_INET;
    server_addres.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addres.sin_addr);

    // Pripojenie k serveru
    if (connect(socket_tcp, (struct sockaddr *) &server_addres, sizeof(server_addres)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    // Citanie zo standratneho vstupu a odosielanie na server
    while (fgets(buffer, sizeof(buffer), stdin)) {
        send(socket_tcp, buffer, strlen(buffer), 0);

        int received_bytes = recv(socket_tcp, buffer, sizeof(buffer) - 1, 0);
        buffer[received_bytes] = '\0';
        
        // Ukoncenie ak server vrati "BYE"
        if (strncmp(buffer, "BYE", 3) == 0) {
            printf("%s", buffer);
            break;
        }

        printf("%s", buffer);
    }

    // ukoncenie soketu
    close(socket_tcp);
}
