#include "readFile.h"

#include <stdio.h>

float **initMat(int row, int col) {
  float **result = (float **)malloc(row * sizeof(float *));
  for (int i = 0; i < row; i++)
    result[i] = (float *)malloc(col * sizeof(float));
  return result;
}

float **readMat(const char *path, int *row, int *col) {
  FILE *fp;
  fp = fopen(path, "r");
  fscanf(fp, "%d", row);
  fscanf(fp, "%d", col);
  float **res = initMat(*row, *col);
  for (int i = 0; i < *row; i++)
    for (int j = 0; j < *col; j++) fscanf(fp, "%f", &res[i][j]);
  fclose(fp);
  return res;
}

void printMat(float **mat, int row, int col) {
  for (int i = 0; i < row; i++) {
    if (i != 0) printf("\n");
    for (int j = 0; j < col; ++j) {
      printf("%.1f ", mat[i][j]);
    }
  }
  printf("\n");
}

void freeMat(float **mat, int row) {
  for (int i = 0; i < row; ++i) free(mat[i]);
  free(mat);
}

void writeMat(const char *path, float **mat, int row, int col) {
  FILE *fp;
  fp = fopen(path, "w");
  fprintf(fp, "%d ", row);
  fprintf(fp, "%d\n", col);
  for (int i = 0; i < row; i++) {
    if (i != 0) fprintf(fp, "\n");
    for (int j = 0; j < col; j++) {
      fprintf(fp, "%.1f ", mat[i][j]);
    }
  }
  fclose(fp);
}
