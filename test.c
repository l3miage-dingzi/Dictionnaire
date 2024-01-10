#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include "table.h"
#include "commande.h"
#include "fonction.h"


int main(){
    int n;
    printf("nombre de fils:");
    scanf("%d",&n);
    //creer n+1 pipes
    int** pipes = (int **)malloc(sizeof(int[2]) * (n+1));
    for(int i=0;i<n+1;i++){
        pipes[i] = malloc(sizeof(int) * 2);
        if(pipe(pipes[i])==-1){
            printf("erreur pipe");
            return 1;
        }
    }
    //commande pour envoyer 
    Commande* commande=(Commande*) malloc(sizeof(Commande));
    init_commande(commande);
    //creer n processus
    for(int i=0;i<n;i++){
        switch(fork()){
            case -1:
                printf("erreur fork");
                exit(0);
                break;
            case 0:
                close_pipes(pipes,i,n);
                if(i==0){
                    list(commande,pipes[n-1],pipes[i],pipes[n],n,i);
                }else{
                    list(commande,pipes[i-1],pipes[i],pipes[n],n,i);
                }
                break;
            default:
                break;
        }
    }
    close_pipes(pipes,n,n);//fermer pipes inutile
    int nb = 0;
    while(nb < n){//verifier que tous les processus sont préparé
        read(pipes[n][0],commande,sizeof(Commande));
        nb++;
    }
    controller(commande,pipes,n);
    for(int i=0;i<n;i++){
        wait(NULL);
    }
    return 0;
}