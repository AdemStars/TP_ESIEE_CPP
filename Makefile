table:
	gcc -o TP1 src/TP1.c -Wall
	gcc -o tablemulti src/tablemulti.c -Wall

clean :
	rm TP1
	rm tablemulti

test:
	echo " ceci est un test pour git"

run:
	./TP1 2 20
	./tablemulti
	
	

