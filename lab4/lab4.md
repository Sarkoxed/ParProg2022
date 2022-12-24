# НИЯУ МИФИ. Лабораторная работа №4. Соколов Александр, Б20-505. 2022.
## Среда разработки
<pre>System:
  Kernel: 6.1.1-arch1-1 arch: x86_64 bits: 64 compiler: gcc v: 12.2.0
    Desktop: MATE v: 1.26.0 Distro: Arch Linux
Memory:
  RAM: total: 14.58 GiB used: 8.29 GiB (56.9%)
  Array-1: capacity: 16 GiB slots: 2 EC: None max-module-size: 8 GiB
    note: est.
  Device-1: DIMM 0 type: DDR4 size: 8 GiB speed: 2400 MT/s
  Device-2: DIMM 0 type: DDR4 size: 8 GiB speed: 2400 MT/s
CPU:
  Info: quad core model: AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx
    bits: 64 type: MT MCP arch: Zen/Zen+ note: check rev: 1 cache: L1: 384 KiB
    L2: 2 MiB L3: 4 MiB
  Speed (MHz): avg: 1575 high: 2100 min/max: 1400/2100 boost: enabled cores:
    1: 1400 2: 1400 3: 1400 4: 2100 5: 1400 6: 2100 7: 1400 8: 1400
    bogomips: 33551
  Flags: avx avx2 ht lm nx pae sse sse2 sse3 sse4_1 sse4_2 sse4a ssse3 svm
OpenMP:
	gcc: 201511
</pre>

```c
Version: 201511
Date(m/y): 11/2015
Number of processors: 8
Number of max threads: 8
Dynamic is enabled?: 0
Timer precision: 1e-09s
Nested parallelism enabled?: 0
Max number of nested active parallel regions: 1
-1503900603
Schedule kind is , chunk size is 32549

```
## Значение директив
<code>#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(guided, 2)
</code>

```c
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;

int main(){
    int n;
    omp_init_lock(&lock);
    int g = 0;
    int threads = 500;
#pragma omp parallel for private(n) shared(g) num_threads(threads)
    for(int i = 0; i < threads; i++){
        n = omp_get_thread_num();
//        printf("I am section %d\n", n);
        if (n % 2 == 0){
            omp_set_lock(&lock);
            g += n;
            omp_unset_lock(&lock);
        }
    }
    omp_destroy_lock(&lock);
    printf("%d\n", g);
}

```
## Экспериментальные вычисления

Static
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_1.png)


Static1
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_2.png)


Static2
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_3.png)


Dynamic
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimedynamic_1.png)


Dynamic2
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimedynamic_2.png)


Guieded1
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimeguided_1.png)


Guided2
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimeguided_2.png)

## Заключение
## Приложение
### Оценка работы параллельной программы

```c
#include <omp.h> 
#include <stdio.h>

char* sched_type(int sched){
    switch(sched){
        case 0x1:
            return "static";
        case 0x2:
            return "dynamic";
        case 0x3:
            return "guided";
        case 0x4:
            return "auto";
        default:
            return "";
    }
}

int main(){
    printf("Version: %d\n", _OPENMP);
    printf("Date(m/y): %d/%d\n", _OPENMP % 100, _OPENMP / 100);
    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Number of max threads: %d\n", omp_get_max_threads());
    printf("Dynamic is enabled?: %d\n", omp_get_dynamic());
    printf("Timer precision: %gs\n", omp_get_wtick());
    printf("Nested parallelism enabled?: %d\n", omp_get_nested());     
    printf("Max number of nested active parallel regions: %d\n", omp_get_max_active_levels());
    omp_sched_t kind;
    int chunk_size;
    printf("%d\n", kind);
    printf("Schedule kind is %s, chunk size is %d\n", sched_type((int)kind), chunk_size);
}

```

```c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void new_array(int *, unsigned int *, int);

void new_array(int *array, unsigned int *random_seed, int count) {
  srand(*random_seed);
  int i;
  for (i = 0; i < count; i++) {
    array[i] = rand();
  }
  *random_seed += rand();
}

int main() {
  const int count = 10000000;
  unsigned int random_seed = 920214;
  const int num_exp = 10;
  const int thread_bound = 16;

  int **arrays = NULL;
  int *array = NULL;
  int index, i, target, threads;

  double t1, t2, res;
  
  fprintf(stderr, "1\n");
  arrays = (int **)calloc(num_exp, sizeof(int *));
  for (int t = 0; t < num_exp; t++) {
    array = (int *)calloc(count, sizeof(int));
    new_array(array, &random_seed, count);
    arrays[t] = array;
  }

  for (threads = 1; threads <= thread_bound; threads++) {
    fprintf(stderr, "curthreads: %d\n", threads);
    res = 0.0;
    for (int j = 0; j < num_exp; j++) {
      fprintf(stderr, "Exp: %d\n", j);
      target = arrays[j][rand() % count];
      index = count + 1;

      t1 = omp_get_wtime();
#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(guided, 2)
      for (i = 0; i < count; i++) {

        if (arrays[j][i] == target) {
          index = i;
        }
      }
      t2 = omp_get_wtime();
      res += t2 - t1;
    }
    res /= (double)(num_exp);
    fprintf(stdout, "(%d, %g), ", threads, res);
  }

  for (int t = 0; t < num_exp; t++) {
    free(arrays[t]);
  }
  free(arrays);
  return (0);
}

```
