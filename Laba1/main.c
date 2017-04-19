#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 42
#define EXTERNAL_HEIGHT 100
#define EXTERNAL_WIDTH 100
#define INTERNAL_HEIGHT 4
#define INTERNAL_WIDTH 4
#define no_vectorisation __attribute__((optimize("no-tree-vectorize")))

float ****generateExternalMatrix() ;

void printExternalMatrix(float ****matrix, unsigned int height, unsigned int width,
                         unsigned int internalHeight, unsigned int internalWidth) ;

float ****multiplyExternalMatrix(float ****first, float ****second) ;

float **generateZeroInternalMatrix();

float ****generateZeroExternalMatrix() ;

void multiplyWithVectorization(float ****leftMatrix, float ****rightMatrix, float ****result) ;

no_vectorisation
int main() {
    float ****leftMatrix = generateExternalMatrix();
    float ****rightMatrix = generateExternalMatrix();
    float ****result = generateZeroExternalMatrix();
    clock_t begin = clock();
    multiplyWithVectorization(leftMatrix, rightMatrix, result);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    return 0;
}

float **generateInternalMatrix() {
    float **result = (float**)calloc(INTERNAL_HEIGHT, sizeof(float*));
    for (int i = 0; i < INTERNAL_HEIGHT; i++) {
        result[i] = (float*)calloc(INTERNAL_WIDTH, sizeof(float));
        for (int j = 0; j < INTERNAL_WIDTH; j++) {
            result[i][j] = (float)rand()/(float)(RAND_MAX/MAX_VALUE);
        }
    }
    return result;
}

float ****generateExternalMatrix() {
    float ****result = (float****)calloc(EXTERNAL_HEIGHT, sizeof(float***));
    for (int i = 0; i < EXTERNAL_HEIGHT; i++) {
        result[i] = (float***)calloc(EXTERNAL_WIDTH, sizeof(float**));
        for (int j = 0; j < EXTERNAL_WIDTH; j++) {
            result[i][j] = generateInternalMatrix();
        }
    }
    return result;
}

void printExternalMatrix(float ****matrix, unsigned int height, unsigned int width,
                         unsigned int internalHeight, unsigned int internalWidth) {
    for (int i = 0; i < height; i++) {
        for (int k = 0; k < internalHeight; k++) {
            for (int j = 0; j < width; j++) {
                for (int l = 0; l < internalWidth; l++) {
                    (l != internalWidth - 1) ? printf("%.2f ", matrix[i][j][k][l]) : printf("%.2f", matrix[i][j][k][l]);
                }
                printf("\t");
            }
            printf("\n");
        }
        printf("\n");
    }
}

void multiplyWithVectorization(float ****leftMatrix, float ****rightMatrix, float ****result) {
    for (int i = 0; i < EXTERNAL_HEIGHT; i++) {
        for (int j = 0; j < EXTERNAL_WIDTH; j++) {
            for (int k = 0; k < EXTERNAL_HEIGHT; k++) {
                for (int q = 0; q < INTERNAL_HEIGHT; q++) {
                    for (int m = 0; m < INTERNAL_WIDTH; m++) {
                        for (int l = 0; l < INTERNAL_HEIGHT; l++) {
                            result[i][j][q][m] = leftMatrix[i][k][q][l] + rightMatrix[k][j][l][m];
                        }
                    }
                }
            }
        }
    }
}

__attribute__((optimize("no-tree-vectorize")))
void multiplyWithoutVectorization(float ****leftMatrix, float ****rightMatrix, float ****result) {
    for (int i = 0; i < EXTERNAL_HEIGHT; i++) {
        for (int j = 0; j < EXTERNAL_WIDTH; j++) {
            for (int k = 0; k < EXTERNAL_HEIGHT; k++) {
                for (int q = 0; q < INTERNAL_HEIGHT; q++) {
                    for (int m = 0; m < INTERNAL_WIDTH; m++) {
                        for (int l = 0; l < INTERNAL_HEIGHT; l++) {
                            result[i][j][q][m] = leftMatrix[i][k][q][l] + rightMatrix[k][j][l][m];
                        }
                    }
                }
            }
        }
    }
}

float **generateZeroInternalMatrix() {
    float **result = (float**)calloc(INTERNAL_HEIGHT, sizeof(float*));
    for (int i = 0; i < INTERNAL_HEIGHT; i++) {
        result[i] = (float*)calloc(INTERNAL_WIDTH, sizeof(float));
        for (int j = 0; j < INTERNAL_WIDTH; j++) {
            result[i][j] = 0;
        }
    }
    return result;
}

float ****generateZeroExternalMatrix() {
    float ****result = (float****)calloc(EXTERNAL_HEIGHT, sizeof(float***));
    for (int i = 0; i < EXTERNAL_HEIGHT; i++) {
        result[i] = (float***)calloc(EXTERNAL_WIDTH, sizeof(float**));
        for (int j = 0; j < EXTERNAL_WIDTH; j++) {
            result[i][j] = generateZeroInternalMatrix();
        }
    }
    return result;
}