<h1>НИЯУ МИФИ. Лабораторная работа №1. Соколов Александр, Б20-505. 2022</h1>

<h2>Используемая система</h2>

<pre>CPU:
  Info: quad core model: AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx
    bits: 64 type: MT MCP arch: Zen/Zen+ note: check rev: 1 cache: L1: 384 KiB
    L2: 2 MiB L3: 4 MiB
  Speed (MHz): avg: 1662 high: 2100 min/max: 1400/2100 boost: enabled
    cores: 1: 2100 2: 2100 3: 2100 4: 1400 5: 1400 6: 1400 7: 1400 8: 1400
    bogomips: 33549
</pre>
<h3>Среда разработки</h3>
<p>Язык программирования C</p>
<h3>Версия OpenMP</h3>
<ul>
    <li><b>gcc</b>:   201511</li>
    <li><b>clang</b>: 201811</li>
</ul>

<h3>Временная оценка алгоритма</h3>
p - количество используемых тредов<br>
n - длина массива чисел</p>
<ul>
    <li>
        <b>"Время работы"</b>
        <ul>
            <li><b>Последовательный:</b>   O(n)</li>
            <li><b>Параллельный:</b>   O(n/p)</li>
        </ul>
    </li>
    <li><b>Ускорение:</b> O(p)</li>
    <li><b>Эффективность:</b> O(1)</li>
</ul>
<h3>Анализ алгоритма</h3>
<h4>Принцип работы</h4>
Приведенный алгоритм идет по массиву чисел и сравнивает их с нынешним для данного треда максимальным элементом. Если элемент оказывается больше, то он переменной max присваивается значение этого элемнта, иначе не присваивается.
После выполнения итерационной части треда, полученное внутри треда значение max сравнивается с max из shared области и из них выбирается наибольший. В итоге наибольший элемент будет в переменной max из shared области.
<h4>Значение директив</h4>
<code>#pragma omp parallel num_threads(threads) shared(array, count) reduction(max: max) default(none)</code><br>
Задается параллельная обасть, с количеством тредов threads. Переменные <b>array</b> и <b>count</b> объявляются общими для всех тредов и непараллельной части алгоритма. Все новые переменные без явного указания класса не разрешены.<br>
Область - <code>цикл for + printf("-- My lmax is: %d;\n", max)</code><br>
Если бы ее не было то цикл просто выполнился бы последовательно.<br><br>
<code>#pragma omp for</code><br>
Задается директива относящаяся к циклу for идущему сразу после нее, выполняется распараллеливагие цикла с дефолтным значением schedule.<br>
Область - <code>цикл for</code><br>
Если бы ее не было то цикл выполнился бы thread раз, каждый раз находя один и тот же максимальный элемент.<br><br>
<h3>Экспериментальные вычисления</h3>
<ul>
    <li>
        Последовательный алгоритм
        <ul>
            <li>Average number of operations: 10000015</li>
            <li>Average time spent: 0.022116 sec</li>
        </ul>
    </li>
</ul>
<h3>Ускорение и эффективность</h3>

![AvgTime](https://user-images.githubusercontent.com/75146596/193953416-cba34470-71ea-4de8-8c0e-ba05ec835bd8.png)

![Acceleration](https://user-images.githubusercontent.com/75146596/193953423-793cf414-58f7-45b9-bd71-599c39e90f75.png)

![Efficiency](https://user-images.githubusercontent.com/75146596/193953426-f4ac8aaa-0f24-4f74-a06d-21a9fb92e559.png)


<h3>Заключение</h3>
В данной работе я ознакомился с азами использования библиотеки OpenMP в языке программирования C. Также установил, что не всегда ожидаемое время работы и ускорение будут совпадать с реальными показателями. По графикам видно, что после 8 тредов прекращается рост ускорения и среднее время выполнения не уменьшается.
<h3>Приложение</h3>
<h4>Оценка работы последовательной программы</h4>

```c

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    for(int j=0; j < count; j++){
        array[j] = rand();
    }
    *random_seed += rand();
}

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    unsigned int random_seed = 920215; ///< RNG seed
    const int num_exp = 20;

    int* array = NULL;                 ///< The array we need to find the max in

    array = (int*)calloc(count, sizeof(int));

    int ops = 0;
    int max;

    for(int i = 0; i < num_exp; i++){
        new_array(array, &random_seed, count);
        max = -1;
        for(int j = 0; j < count; j++){
            if(array[j] > max){
                max = array[j];
                ops++;
            }
            ops++;
        }
    }
    printf("Average number of comparison operations: %d\n", ops / num_exp);

    double t1, t2, res;
    random_seed = 920215; ///< RNG seed
    for(int i = 0; i < num_exp; i++){
        new_array(array, &random_seed, count);
        max = -1;
        t1 = omp_get_wtime();
        for(int j = 0; j < count; j++){
            if(array[j] > max){
                max = array[j];
            }
        }
        t2 = omp_get_wtime();
        res += t2 - t1;
    }

    printf("Average time spent: %lf\n", res / (double)(num_exp));

    free(array);
    return(0);
}
```
<h4>Оценка работы параллельной программы с разным колчиеством тредов</h4>

```c

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    for(int j=0; j < count; j++){
        array[j] = rand(); 
    }
    *random_seed += rand();
}


int main(int argc, char** argv)
{
    const int count = 10000000;   
    int threads;         
    unsigned int random_seed = 920215; ///< RNG seed
    const int num_exp = 20;
    const int thread_bound = 256;

    int* array = NULL;           
    int** arrays = NULL;
    int  max;                
    double t1, t2, res;

    arrays = (int**)calloc(num_exp, sizeof(int*));
    for(int t = 0; t < num_exp; t++){
        array = (int*)calloc(count, sizeof(int));
        new_array(array, &random_seed, count);
        arrays[t] = array;
    }
    
    for(int threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "curthreads: %d\n", threads);
        res = 0.0;
        for(int j = 0; j < num_exp; j++){
            max = -1;
            t1 = omp_get_wtime();
            #pragma omp parallel num_threads(threads) shared(arrays, count, j) reduction(max: max) default(none)
            {
                #pragma omp for
                for(int i=0; i < count; i++)
                {
                    if(arrays[j][i] > max){ 
                        max = arrays[j][i];
                    }
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
        }
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res); 
    }

    for(int t = 0; t < num_exp; t++){
        free(arrays[t]);
    }
    free(arrays);
    return(0);
}

```
