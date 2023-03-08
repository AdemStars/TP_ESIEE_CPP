##Makefile réaliser par le Groupe 5 (Adi, Lucas D., Landry, Thomas)


compil:
##Permet de créer le répertoire bin; compiler les fichiers listés contenus dans le dossier src; Stocke les binaires dans le dossier bin
	mkdir ./bin
	gcc src/client.c -o ./bin/client -Wall
	gcc src/server.c -o ./bin/server -Wall

clean :
## Permet de supprimer le dossier bin
	rm -r ./bin

info:
	echo "Bienvenue dans la page Info de notre Makefile"
	echo 
	echo "make compil : vous permettra de compiler les fichiers listés dans ce makefile"
	echo
	echo "make clean: Vous permettra de supprimer tous les fichiers contenus dans le répertoire bin"
	echo
	echo "make client: Permet d'executer le binaire client en insérant l'ip server lors de l'execution"
	echo
	echo "make server: Permet d'executer le binaire server"

client:	
#Permet d'exe le binaire client en insérant l'ip server lors de l'execution
	./bin/client $(ip)

server:	
#Permet d'exe le binaire server
	./bin/server	
