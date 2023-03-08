##Makefile réaliser par le Groupe 5 (Adi, Lucas D., Landry, Thomas)


compil:##Permet de compiler les fichiers listés contenus dans le dossier src. Stocke les binaires dans le dossier bin
	gcc src/client.c -o ./bin/client -Wall
	gcc src/server.c -o ./bin/server -Wall

clean :## Permet de supprimer tous les fichiers contenus dans bin
	rm -r ./bin/*

test:
	echo " ceci est un test pour git"

client:#Permet d'exe le binaire client
	./bin/client $(ip)

server:#Permet d'exe le binaire server
	./bin/server	
	

