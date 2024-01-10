#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commande.h"


void init_commande(Commande* commande){
    commande->command=-1;
    commande->key=-1;
    strcpy(commande->data,"");
}

void new_commande(Commande* commande){
    int com;
    int key;
    char data[128];
    init_connande(commande);
    printf("Saisir commande (0 = exit, 1 = set, 2 = lookup, 3 = dump):");
    scanf("%d",&com);
    while(com<0 || com>3){
        printf("Connande inconnu\n");
        printf("Saisir commande (0 = exit, 1 = set, 2 = lookup, 3 = dump):");
        scanf("%d",&com);
    }
    commande->command=com;
    switch(com){
        case 1:
            printf("Saisir la cle (decimal number):");
            scanf("%d",&key);
            while(key<0){
                printf("Key inconnu\n");
                printf("Saisir la cle (decimal number):");
                scanf("%d",&key);
            }
            commande->key=key;
            printf("Saisir la valeur (chaine de caracteres, max 128 chars): ");
            scanf("%s",data);
            while(strlen(data)==0){
                printf("Mot vide\n");
                printf("Saisir la valeur (chaine de caracteres, max 128 chars): ");
                scanf("%s",data);
            }
            //printf("nmsl");
            strcpy(commande->data,data);
            break;
        case 2:
            printf("Saisir la cle (decimal number):");
            scanf("%d",&key);
            while(key<0){
                printf("Key inconnu\n");
                printf("Saisir la cle (decimal number):");
                scanf("%d",&key);
            }
            commande->key=key;
            break;
        default:
            break;
    }
}