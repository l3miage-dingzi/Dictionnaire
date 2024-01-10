test:test.o table.o commande.o fonction.o
	gcc test.o table.o commande.o fonction.o -o test

table.o:table.c table.h
	gcc -c table.c

test.o:test.c
	gcc -c test.c

commande.o:commande.c commande.h
	gcc -c commande.c

fonction.o:fonction.c fonction.h commande.c table.c
	gcc -c fonction.c commande.c table.c

clean :
	rm test test.o table.o fonction.o commande.o

