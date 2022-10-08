#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void new_array(int *array, unsigned int *random_seed, int count) {
  srand(*random_seed);
  for (int j = 0; j < count; j++) {
    array[j] = rand();
  }
  *random_seed += rand();
}

int main(int argc, char **argv) {
  const int count = 10000000;        ///< Number of array elements
  unsigned int random_seed = 920215; ///< RNG seed
  const int num_exp = 20;

  int *array = NULL; ///< The array we need to find the max in

  array = (int *)calloc(count, sizeof(int));

  int ops = 0;
  int max;

  for (int i = 0; i < num_exp; i++) {
    new_array(array, &random_seed, count);
    max = -1;
    for (int j = 0; j < count; j++) {
      if (array[j] > max) {
        max = array[j];
        ops++;
      }
      ops++;
    }
    //        printf("max: %d\n", max);
  }
  printf("Average number of comparison operations: %d\n", ops / num_exp);

  double t1, t2, res;
  random_seed = 920215; ///< RNG seed
  for (int i = 0; i < num_exp; i++) {
    new_array(array, &random_seed, count);
    max = -1;
    t1 = omp_get_wtime();
    for (int j = 0; j < count; j++) {
      if (array[j] > max) {
        max = array[j];
      }
    }
    t2 = omp_get_wtime();
    res += t2 - t1;
    //        printf("max: %d\n", max);
  }

  printf("Average time spent: %lf\n", res / (double)(num_exp));

  free(array);
  return (0);
}
