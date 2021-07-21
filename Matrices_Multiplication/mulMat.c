#include <pthread.h>
#include "readFile.h"

pthread_mutex_t lock;
int rowA, colA, rowB, colB, curLines, noThread = 2, step = -1;
float **matA, **matB, **matRes;

void *multiThreadMatMul(void *arg) {
  pthread_mutex_lock(&lock);
  step++;
  pthread_mutex_unlock(&lock);

  // Compute Matrices multipication in a given range
  for (int i = step * curLines; i < rowA && i < (step + 1) * curLines; i++)
    for (int j = 0; j < colB; j++) {
      matRes[i][j] = 0;
      for (int k = 0; k < colA; k++) matRes[i][j] += matA[i][k] * matB[k][j];
    }
}

int main() {
  // Prepare data
  matA = readMat("inputA.txt", &rowA, &colA),
  matB = readMat("inputB.txt", &rowB, &colB), matRes = initMat(rowA, colB);

  printf("Number of threads: ");
  scanf("%d", &noThread);

  if (rowA % noThread != 0)
    curLines = rowA / noThread + 1;
  else
    curLines = rowA / noThread;
  // printMat(matA, rowA, colA);
  // printMat(matB, rowB, colB);

  // Check for multiplicability
  if (colA != rowB) {
    printf(
        "Number of matrix A's column does not equal to number of matrix B's "
        "row.\n");
    return 1;
  }

  clock_t begin = clock();
  
  // Create thread and process
  pthread_t *threads = (pthread_t *)malloc(noThread * sizeof(pthread_t));

  int *status;//check if thread created successfully
  status = (int *)malloc(noThread * sizeof(int));

  for (int i = 0; i < noThread; i++)
    status[i] = pthread_create(&threads[i], NULL, multiThreadMatMul, NULL);

  for (int i = 0; i < noThread; i++)
    if (!status[i])
      pthread_join(threads[i], NULL);
    else
      printf("Error in thread %d", i);

  clock_t end = clock();

  // Display
  // printMat(matRes, rowA, colB);

  // Write result
  writeMat("output.txt", matRes, rowA, colB);

  // Deallocate memory
  free(status);
  free(threads);
  freeMat(matRes, rowA);
  freeMat(matA, rowA);
  freeMat(matB, rowB);

  float time_spent = (float)(end - begin);
  printf("Done in %.3f clocks\n", time_spent);
  return 0;
}
