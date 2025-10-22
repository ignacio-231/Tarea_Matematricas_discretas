#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> // Necesario para rand() y srand()

#define MAX_LINE_LENGTH 256

// --- Prototipos de funciones ---
void createMatrix(char*** matrix, int size);
void freeMatrix(char** matrix, int size);
void readGraph(const char* fileName, int* nNodes, char*** matrix);
void crear_matriz_orientada(char*** matriz_ptr, char*** matriz_salida_ptr, int nNodes);
void findShortestPath(char** matrix, int nNodes, char v1, char v2);

// --- Implementación de funciones ---

void createMatrix(char*** matrix, int size) {
    *matrix = (char**)malloc(size * sizeof(char*));
    if (*matrix == NULL) {
        perror("Error en malloc para filas");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        (*matrix)[i] = (char*)malloc(size * sizeof(char));
        if ((*matrix)[i] == NULL) {
            perror("Error en malloc para columnas");
            exit(1);
        }
        for (int j = 0; j < size; j++) {
            (*matrix)[i][j] = 0;
        }
    }
}

void freeMatrix(char** matrix, int size) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}


void readGraph(const char* fileName, int* nNodes, char*** matrix) {
    FILE *file;
    char lineV[MAX_LINE_LENGTH];
    char lineE[MAX_LINE_LENGTH];

    file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    if (fgets(lineV, MAX_LINE_LENGTH, file) == NULL || fgets(lineE, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Error: El archivo está incompleto o vacío.\n");
        fclose(file);
        exit(1);
    }
    fclose(file);

    lineV[strcspn(lineV, "\n\r")] = 0;
    lineE[strcspn(lineE, "\n\r")] = 0;

    char* lineV_copy = strdup(lineV); 
    char* lineE_copy = strdup(lineE);
    
    char* token = strtok(lineV_copy, ",");
    while (token != NULL) {
        (*nNodes)++;
        token = strtok(NULL, ",");
    }

    if (*nNodes == 0) {
        fprintf(stderr, "Error: No se encontraron nodos en el archivo.\n");
        exit(1);
    }

    createMatrix(matrix, *nNodes);

    token = strtok(lineE_copy, ",");
    while (token != NULL) {
        if (strlen(token) >= 2) {
            char valA = token[0] - 'a';
            char valB = token[1] - 'a';
            if (valA >= 0 && valA < *nNodes && valB >= 0 && valB < *nNodes) {
                (*matrix)[valA][valB] = 1;
                (*matrix)[valB][valA] = 1;
            }
        }
        token = strtok(NULL, ",");
    }
    
    free(lineV_copy);
    free(lineE_copy);
}

void crear_matriz_orientada(char*** matriz_ptr, char*** matriz_salida_ptr, int nNodes) {
    
    char** matriz_entrada = *matriz_ptr;

    // Asignar memoria para la nueva matriz de salida
    createMatrix(matriz_salida_ptr, nNodes);

    char** matriz_salida = *matriz_salida_ptr;

    // Iterar sobre la matriz de entrada (solo el triángulo superior)
    for (int i = 0; i < nNodes; i++) {
        for (int j = i + 1; j < nNodes; j++) {
            
            // Si existe una arista no-dirigida en la matriz original...
            if (matriz_entrada[i][j] == 1) {
                
                // ...decidir aleatoriamente la dirección en la nueva matriz.
                if (rand() % 2) {
                    // Crear arista dirigida i -> j
                    matriz_salida[i][j] = 1;
                } else {
                    // Crear arista dirigida j -> i
                    matriz_salida[j][i] = 1;
                }
            }
        }
    }
}

void findShortestPath(char** matrix, int nNodes, char v1, char v2) {
    int startNode = v1 - 'a';
    int endNode = v2 - 'a';

    // Fila 0: distancia desde el inicio; Fila 1: nodo predecesor en el camino
    int pathInfo[2][nNodes];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < nNodes; j++) {
            pathInfo[i][j] = -1; // -1 representa infinito/no visitado
        }
    }

    int queue[nNodes]; // Cola para los nodos a visitar (BFS)
    int visited[nNodes]; // Para marcar nodos ya visitados y en la cola
    for(int i = 0; i < nNodes; i++) visited[i] = 0;

    int head = 0, tail = 0;

    // 1. Iniciar el algoritmo
    pathInfo[0][startNode] = 0; // Distancia a sí mismo es 0
    queue[tail++] = startNode;
    visited[startNode] = 1;

    // 2. Búsqueda en Anchura (BFS)
    while (head < tail) {
        int currentNode = queue[head++]; // Sacar el primer nodo de la cola

        if (currentNode == endNode) {
            break; // Se encontró el destino
        }

        // Explorar vecinos
        for (int neighbor = 0; neighbor < nNodes; neighbor++) {
            if (matrix[currentNode][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = 1;
                pathInfo[0][neighbor] = pathInfo[0][currentNode] + 1; // Distancia es 1 más que el padre
                pathInfo[1][neighbor] = currentNode;                  // Guardar quién lo encontró
                queue[tail++] = neighbor;                             // Añadir vecino a la cola
            }
        }
    }

    // 3. Reconstruir y mostrar el camino
    if (pathInfo[0][endNode] == -1) {
        printf("No se encontró un camino entre %c y %c.\n", v1, v2);
    } else {
        int pathLength = pathInfo[0][endNode] + 1;
        printf("El camino mas corto tiene %d nodos y es:\n", pathLength);

        int path[pathLength];
        int currentNode = endNode;
        for (int i = pathLength - 1; i >= 0; i--) {
            path[i] = currentNode;
            currentNode = pathInfo[1][currentNode];
        }

        for (int i = 0; i < pathLength; i++) {
            printf("%c%s", 'a' + path[i], (i == pathLength - 1) ? "" : " -> ");
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        printf("Error, uso del programa: %s v1 v2 grafo.txt\n", argv[0]);
        return 1;
    }
    
    // Inicializar el generador de números aleatorios
    srand(time(NULL));

    const char v1 = argv[1][0];
    const char v2 = argv[2][0];
    const char* fileName = argv[3];

    int nNodes = 0;
    char** matrix = NULL;
    char** matrix_orientada = NULL;

    readGraph(fileName, &nNodes, &matrix);
    crear_matriz_orientada(&matrix, &matrix_orientada, nNodes);


    // Imprimir matriz de adyacencia original
    printf("Matriz de adyacencia (Original No-Dirigida):\n  ");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < nNodes; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n\n");
    }

    // Imprimir matriz de adyacencia nueva (orientada)
    printf("Matriz de adyacencia (Nueva Dirigida):\n  ");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < nNodes; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < nNodes; j++) {
            printf("%d ", matrix_orientada[i][j]);
        }
        printf("\n\n");
    }
    
    // Camino en el grafo original
    printf("--- Camino en Grafo Original (No-Dirigido) ---\n");
    findShortestPath(matrix, nNodes, v1, v2);
    
    // Camino en el grafo nuevo (orientado)
    printf("\n--- Camino en Grafo Nuevo (Dirigido) ---\n");
    findShortestPath(matrix_orientada, nNodes, v1, v2);

    
    // Liberar ambas matrices
    freeMatrix(matrix, nNodes);
    freeMatrix(matrix_orientada, nNodes);

    return 0;
}