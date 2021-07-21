#include <stdio.h>
#include <stdlib.h>

float **readMat(const char *path, int *row, int *col);
void printMat(float **mat, int row, int col);
void freeMat(float **mat, int row);
void writeMat(const char *path, float **mat, int row, int col);
float **initMat(int row, int col);
