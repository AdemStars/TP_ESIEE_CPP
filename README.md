# TP_ESIEE_CPP



Par groupe de 4, en utilisant un dépôt git partagé, vous utiliserez un makefile pour compiler et lancer votre projet.



Vous créez un interpreteur de commande en mode réseau.

Vous pouvez vous inspirer du code client et serveur suivant https://www.geeksforgeeks.org/socket-programming-cc/.

Vous pouvez utiliser toutes les ressources internet a votre disposition.





Le client:

Lors de son lancement l'adresse IP du serveur sera passée en paramètre du programme, exemple ./client 192.168.1.2

Le client affiche un interpreteur de commande et lors de l'exécution de la commande celle ci est transmise au serveur qui l'exécute et retour le résultat au client.

Le programme doit masquer les signaux SIGINTpour ne pas être fermé par CTRL+c.



Le serveur:

Il écoute sur un port donné 8080

Il exécute les commandes reçue du client.

Il conserve dans un fichier de logs chaque commande qu'il aura traité, mais pas leur résultat.

Il retourne le résultat de chaque commande au client.





Exemple, le client doit pouvoir effectuer la commande "ls /tmp" , le serveur retournera le contenu du répertoire /tmp et conservera la commande dans le fichier log.txt.

