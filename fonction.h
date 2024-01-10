#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void close_pipes(int** pipes, int indice, int n);
void list(Commande* commande, int reading_pipe[], int writing_pipe[], int controller_pipe[], int n, int indice);
void controller(Commande* commande, int** pipes, int n);

#endif