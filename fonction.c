#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "commande.h"
#include "table.h"
#include "fonction.h"

void close_pipes(int** pipes, int indice, int n){
    if(indice == n){
        for(int i=0;i<n;i++){
            close(pipes[i][0]);
        }
        close(pipes[n][1]);
    }else{
        if(indice==0){
            for(int i=1;i<n-1;i++){
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            close(pipes[n-1][1]);
        }else{
            for(int i=0;i<n;i++){
                if(i!=indice && i!=indice-1){
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }
            }
            close(pipes[indice-1][1]);
        }
        close(pipes[indice][0]);
        close(pipes[n][0]);
    }
}

void list(Commande* commande, int reading_pipe[], int writing_pipe[], int controller_pipe[], int n, int indice){
    int sort=0;
    PTable_entry table=(PTable_entry)malloc(sizeof(PTable_entry));
    //ecrire les infos à controller pour justifier la preparation
    write(controller_pipe[1],commande,sizeof(Commande));
    do{
        //recevoir la commande de processus avant
        read(reading_pipe[0],commande,sizeof(Commande));

        switch(commande->command){
            case 0:
                sort=-1;
                break;
            case 1:
                //printf("1");
                //Si la cle dans le processus ,on enregistre ,sinon envoyer les infos à processus suivant
                if(commande->key%n==indice){
                    if(key_existe(table, commande->key)){
                        //Si la cle deja existe ,on ne store pas, donc ici sans modifier la fonction store,
                        // on ne peut pas modifier une valeur d'une cle déja existante
                        //printf("key existe");
                        strcpy(commande->data,"key existe");
                    }else{
                        store(&table,commande->key,commande->data);
                        strcpy(commande->data,"store ok");
                        //printf("store ok");
                    }
                    //envoyer le resultat à controller
                    write(controller_pipe[1],commande,sizeof(Commande));
                }else{
                    write(writing_pipe[1],commande,sizeof(Commande));
                }
                break;
            case 2:
            //Si la cle dans le processus ,on lookup ,sinon envoyer les infos à processus suivant
                if(commande->key%n==indice){
                        if(key_existe(table, commande->key)){
                            //printf("Valeur trouvee = %s",lookup(table, commande->key));
                            strcpy(commande->data,lookup(table, commande->key));
                        }else{
                            //printf("Pas de valeur trouvee");
                            strcpy(commande->data,"Pas de valeur trouvee");
                        }
                        //envoyer le resultat à controller
                        write(controller_pipe[1],commande,sizeof(Commande));
                    }else{
                        write(writing_pipe[1],commande,sizeof(Commande));
                    }
                break;
            case 3:
                write(writing_pipe[1],commande,sizeof(Commande));
                read(reading_pipe[0],commande,sizeof(Commande));
                if(indice!=0){   //attend le processus precedent
                    read(reading_pipe[0],commande,sizeof(Commande));
                }
                printf("process %d \n:",getpid());
                display(table);
                //envoyer le resultat au controller
                write(controller_pipe[1],commande,sizeof(Commande));
                //envoyer la commande au processus suivant
                if(indice!=n-1){
                    //printf("nmsl");
                    write(writing_pipe[1],commande,sizeof(Commande));
                }
                break;
            default:
                break;
        }
    }while(sort==0);
    //printf("exit");
    exit(0);
}

void controller(Commande* commande, int** pipes, int n){
    int nb=0;
    int sort=0;
    do{
        //printf("nmsl");
        new_commande(commande);
        switch(commande->command){
            case 0:
            case 3://envoyer la commande en meme temps
                for(int i=0;i<n;i++){//envoyer la commande à tous les processus en passant par les tubes
                    write(pipes[i][1],commande,sizeof(Commande));
                }
                nb=0;
                while(nb<n){//verifier que tous les processus reçoient la commande 
                    read(pipes[n][0],commande,sizeof(Commande));
                    nb++;
                }
                if(commande->command==0){
                    //printf("1");
                    sort=1;
                }
                break;
            default:
                write(pipes[n-1][1],commande,sizeof(Commande));//ecrire la commande à pipes n-1
                read(pipes[n][0],commande,sizeof(Commande));//recevoir le resultat
                //afficher le resultat
                if(strlen(commande->data)!=0){
                    if(commande->command==2 && strcmp(commande->data,"Pas de valeur trouvee")){
                        printf("Valeur trouvee = ");
                        printf("%s\n",commande->data);
                    }else{
                        printf("%s\n",commande->data);
                    }
                    
                }
                break; 
        }
    }while(sort==0);
    
}