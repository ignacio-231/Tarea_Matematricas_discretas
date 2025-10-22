#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256

void createMatrix(char*** matrix, int size) {
    *matrix = (char**)malloc(size * sizeof(char*));

    for (int i = 0; i < size; i++) {
        (*matrix)[i] = (char*)malloc(size * sizeof(char));
    
        for (int j = 0; j < size; j++) {
            (*matrix)[i][j] = 0;
        }
    }
}

void readGraph(const char* fileName, int* nNodes, char*** matrix) {
    FILE *file;
    char lineV[MAX_LINE_LENGTH];
    char lineE[MAX_LINE_LENGTH];

    file = fopen(fileName, "r");

    if (file == NULL) {
        perror("Error reading file\n");
        exit(1);
    }

    fgets(lineV, MAX_LINE_LENGTH, file);
    fgets(lineE, MAX_LINE_LENGTH, file);

    char* token = strtok(lineV, ",");
    while (token != NULL) {
        (*nNodes)++;
        token = strtok(NULL, ",");
    }

    createMatrix(matrix, *nNodes);

    token = strtok(lineE, ",");
    while (token != NULL) {
        char valA = (token[0]) - 'a';
        char valB = (token[1]) - 'a';

        (*matrix)[valA][valB] = 1;
        (*matrix)[valB][valA] = 1;

        token = strtok(NULL, ",");
    }

    fclose(file);
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        printf("Error, uso del programa: ./main v1 v2 <grafo.txt>\n");
        return 1;
    }

    const char v1 = *argv[1];
    const char v2 = *argv[2];
    const char* fileName = argv[3];

    int nNodes = 0;
    int eje_x = 0;
    char** matrix = NULL;

    readGraph(fileName, &nNodes, &matrix);

    //llena de null la tabla
    int dijkstra[2][nNodes];
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < nNodes; j++){
        dijkstra[i][j] = -1; //equivalente a null
        }
    }


    printf("  ");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < nNodes; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("(Matriz de adyacencia)\n\n");


    //calculo del camino mas corto
    int inicio = v1 - 'a';
    int final = v2 - 'a';
    int peso = 1;
    int recoridos[nNodes];
    for(int i = 0; i < nNodes; i++){
        recoridos[i] = -1; //equivalente a null
    }
    recoridos[0] = inicio;
    int indice= 1;
    bool repetido = false;

    dijkstra[0][inicio] = 0; //valor inicial

    for(int i = 0; i < nNodes; i++){//recore los vertices agregados
        for(int j = 0; j < nNodes; j++){//recorre las aristas de cada vertice
            if(matrix[recoridos[i]][j] == 1){
                for(int z = 0; z < indice; z++){
                    if (recoridos[z] == j){
                        repetido = true;
                    }
                }
                if (repetido == false){
                    recoridos[indice] = j;
                    indice ++;
                    dijkstra[0][j] = dijkstra[0][recoridos[i]] + 1;
                    dijkstra[1][j] = recoridos[i];
                    if(final == j){//termina el ciclo si se encuentera el camino
                        break;
                    }
                }
                else{
                    repetido = false;
                }
            }
        }
        if(recoridos[indice-1] == final){ //ve si el ultimo vertice encontrado es el final
        break;
        }
    }



    int largo_camino;

    if (dijkstra[0][final] < 1) {
        largo_camino = 1;
    } else {
        largo_camino = dijkstra[0][final] + 1;
    }
    printf("El camino mas corto tiene largo %d y es:\n", largo_camino);

    int camino[largo_camino];
    camino[0] = final;
    int actual = final;

    for(int i = 1; i < largo_camino; i++){ //rellena la matriz que almacena el camino, pero al reves
        actual = dijkstra[1][actual];
        camino[i] = actual;
    }

    printf("%c", 'a' + camino[largo_camino - 1]);

    for(int i = largo_camino - 2; i >= 0; i--){ //printea en orden inverso reves
        printf(" -> %c", 'a' + camino[i]);
    }
    printf("\n");

    return 0;
}