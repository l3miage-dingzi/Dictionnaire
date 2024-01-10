#ifndef COMMANDE_H
#define COMMANDE_H

typedef struct Connamde{
    int command;    
    int key;        
    char data[128]; 
} Commande;

void init_commande(Commande* commamde);
void new_commande(Commande* commamde);


#endif