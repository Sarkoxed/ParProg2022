#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int*, unsigned int*, int);
double shellsort(int*, int, int);

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    int r = rand()  + count;
    for(i=0; i < count; i++) { 
      array[i] = r - i; 
    }
    *random_seed += rand();
}

double shellsort(int* array, int count, int threads){
    double t1, t2;
    int i, j, tmp, part;
    t1 = omp_get_wtime();
    for(int gap = count/2; gap > 0; gap /= 2){
#pragma omp parallel for shared(gap, count, array) private(i, j, tmp, part) default(none) num_threads(threads)
        for(i = 0; i < gap; i++){
            for(part = i + gap; part < count; part += gap){
                for(j=part; j>i  && array[j-gap] > array[j]; j-=gap){
                    tmp = array[j];
                    array[j] = array[j-gap];
                    array[j-gap] = tmp;
                }
            }
        }
    }
    t2 = omp_get_wtime();
    return t2 - t1;
}   

int main(){
    const int count = 1000000;
    unsigned int random_seed = 1337;
    const int num_exp = 10;
    const int thread_bound = 16;
    int threads;
    
    int*** arrays = NULL;
    int* array = NULL;

    arrays = (int***)calloc(thread_bound, sizeof(int**));
    for(int t = 0; t < thread_bound; t++){ 
        arrays[t] = (int**)calloc(num_exp, sizeof(int*));
        for(int e = 0; e < num_exp; e++){
            arrays[t][e] = (int*)calloc(count, sizeof(int));
            new_array(arrays[t][e], &random_seed, count);
        }
        random_seed = 1337;
    }

    double res = 0.0;

    for(threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "Number of threads: %d/%d\n", threads, thread_bound);
        res = 0.0;
        for(int e = 0; e < num_exp; e++){ 
            fprintf(stderr, "Number of experiment: %d/%d\n", e+1, num_exp);
            array = arrays[threads-1][e];
            res += shellsort(array, count, threads);
        }
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res);
    }
    
    for(int t = 0; t < thread_bound; t++){ 
        for(int e = 0; e < num_exp; e++){
            free(arrays[t][e]);
        }
        free(arrays[t]);
    }
    free(arrays);
    return 0;
}
