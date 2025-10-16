#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char** matrix = NULL;

    readGraph(fileName, &nNodes, &matrix);

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

    return 0;
}


