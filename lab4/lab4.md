# НИЯУ МИФИ. Лабораторная работа №4. Соколов Александр, Б20-505. 2022.
## Среда разработки
<pre>System:
  Kernel: 6.0.2-arch1-1 arch: x86_64 bits: 64 compiler: gcc v: 12.2.0
    Desktop: GNOME v: 42.5 Distro: Arch Linux
Memory:
  RAM: total: 15.57 GiB used: 2.78 GiB (17.8%)
  Array-1: capacity: 32 GiB slots: 4 EC: None max-module-size: 8 GiB
    note: est.
  Device-1: DIMM_A1 type: no module installed
  Device-2: DIMM_A2 type: DDR3 size: 8 GiB speed: 1600 MT/s
  Device-3: DIMM_B1 type: no module installed
  Device-4: DIMM_B2 type: DDR3 size: 8 GiB speed: 1600 MT/s
CPU:
  Info: quad core model: Intel Core i5-4690 bits: 64 type: MCP arch: Haswell
    rev: 3 cache: L1: 256 KiB L2: 1024 KiB L3: 6 MiB
  Speed (MHz): avg: 3492 high: 3493 min/max: 800/3900 cores: 1: 3492 2: 3492
    3: 3493 4: 3493 bogomips: 27945
  Flags: avx avx2 ht lm nx pae sse sse2 sse3 sse4_1 sse4_2 ssse3 vmx
OpenMP:
	gcc: 201511
</pre>

1. Определить дату принятия используемого стандарта OpenMP
```
Version: 201511
Date(m/y): 11/2015
```

2. Количество доступных процессоров - 4, количество доступных потоков - 4
```
Number of processors: 4
Number of max threads: 4
```

3. `dynamic` - показавает, возможно ли динамически настравивать количество используемых потоков во время работы программы
```
Dynamic is enabled?: 0
```

4. Разрешение таймера:
```
Timer precision: 1e-09s
```

5. Особенности работы со вложенными параллельными областями 
```
Nested parallelism enabled?: 0
Max number of nested active parallel regions: 1
```

6. Текущее распределение нагрузки:
```
Schedule kind is dynamic, chunk size is 1
```

## * <strong>все данные получены с помощью программы 1 в приложении</strong>

7. Пример использования замков
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
Если честно необходимости исользования lock'ов тут нет. Спокойно можно было бы обойтись atomic. Однако если все же использовать локи, то без них доступ к переменной g был бы у всех потоков одновременно, что привело бы к неопределенному поведению. Замки же помогают разграничить доступ к ней, чтобы в данный момент времени ее мог изменять только поток обладающий замком, а остальные ждут пока замок не освободится.

## Значение директив
<code>#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(static)
</code>

## Экспериментальные вычисления
Размеры используемых чанков:
- Static
  - default
  - 10
  - 1000
- Dynamic, 
  - default
  - 10
  - 1000
- Guided
  - default
  - 10
  - 1000

Static
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_1.png)


Static10
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_2.png)


Static1000
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimestatic_3.png)


Dynamic
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimedynamic_1.png)


Dynamic10
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimedynamic_2.png)


Dynamic1000
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimedynamic_3.png)


Guieded
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimeguided_1.png)


Guided10
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimeguided_2.png)

Guided1000
![AvgTime](https://github.com/Sarkoxed/ParProg2022/blob/master/lab4/graphs/AvgTimeguided_3.png)

## Заключение
## Приложение
### Оценка работы параллельной программы

#### Программа 1
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
    omp_get_schedule(&kind, &chunk_size);
    printf("Schedule kind is %s, chunk size is %d\n", sched_type((int)kind), chunk_size);
}

```

#### Программа 2
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
  
//  fprintf(stderr, "1\n");
  arrays = (int **)calloc(num_exp, sizeof(int *));
  for (int t = 0; t < num_exp; t++) {
    array = (int *)calloc(count, sizeof(int));
    new_array(array, &random_seed, count);
    arrays[t] = array;
  }

  for (threads = 1; threads <= thread_bound; threads++) {
//    fprintf(stderr, "curthreads: %d\n", threads);
    res = 0.0;
    for (int j = 0; j < num_exp; j++) {
//      fprintf(stderr, "Exp: %d\n", j);
      target = arrays[j][rand() % count];
      index = count + 1;

      t1 = omp_get_wtime();
#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(static)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(static, 1)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(static, 2)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(dynamic, 1)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(dynamic, 2)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(guided, 1)
//#pragma omp parallel for shared(arrays, count, target, j) default(none) private(i) num_threads(threads) reduction(min : index) schedule(guided, 2)
      for (i = 0; i < count; i++) {

        if (arrays[j][i] == target) {
          index = i;
        }
      }
      t2 = omp_get_wtime();
      res += t2 - t1;
      //fprintf(stdout, "%g, ",t2 - t1);
    }
    //fprintf(stdout, "\n");
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
