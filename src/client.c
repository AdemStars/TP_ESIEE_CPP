#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char command[1024] = {0};
    char* ip_address = NULL;

    // Vérifier si l'adresse IP du serveur a été passée en paramètre
    if (argc < 2) {
        printf("Usage: %s ip_address\n", argv[0]);
        return -1;
    }
    ip_address = argv[1];

    // Créer la socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Erreur de création de la socket \n");
        return -1;
    }

    // Configuration de l'adresse du serveur
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir l'adresse IP en binaire et l'assigner à l'adresse du serveur
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        printf("\nAdresse IP invalide ou non supportée\n");
        return -1;
    }

    // Établir la connexion avec le serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnexion échouée \n");
        return -1;
    }

    // Masquer les signaux SIGINT pour ne pas être fermé par CTRL+c
    signal(SIGINT, SIG_IGN);

    // Afficher l'interpréteur de commandes
    while(1) {
        printf("$ ");
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
        memset(command, 0, sizeof(command));
        fgets(command, 1024, stdin);

        // Envoyer la commande au serveur
        send(sock, command, strlen(command), 0);

        // Recevoir la réponse du serveur
        read(sock, buffer, 1024);
        printf("%s", buffer);
    }

    return 0;
}