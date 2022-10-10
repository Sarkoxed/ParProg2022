#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int*, unsigned int*, int);
void print_array(int*, int, int);

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    for(i=0; i < count; i++) { 
      array[i] = rand(); 
    }
    *random_seed += rand();
}
    
int main(){
    const int count = 100000;
    unsigned int random_seed = 1337;
    const int num_exp = 10;
    const int thread_bound = 8;
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

    double t1, t2, res = 0.0;

    for(threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "Number of threads: %d/%d\n", threads, thread_bound);
        res = 0.0;
        for(int e = 0; e < num_exp; e++){ 
            fprintf(stderr, "Number of experiment: %d/%d\n", e+1, num_exp);
            array = arrays[threads-1][e];
            t1 = omp_get_wtime();
            for(int gap = count/2; gap > 0; gap /= 2){
                for(int i = gap; i < count; i+=gap){
                    int bnd = (i + gap >= count)?count:i+gap;

 //                   if (gap >= threads && gap != 1){
#pragma omp parallel for shared(gap, array, i, bnd) num_threads(threads)
                        for(int k = i; k < bnd; k++){
                            for(int j = k; j >= gap && array[j-gap] > array[j] ; j -= gap){
                                int tmp = array[j];
                                array[j] = array[j-gap];
                                array[j-gap] = tmp;
                            }
                        }
//                    }else{
//                        for(int k = i; k < bnd; k++){
//                            for(int j = k; j >= gap && array[j-gap] > array[j]; j -= gap){
//                                int tmp = array[j];
//                                array[j] = array[j-gap];
//                                array[j-gap] = tmp;
//                            }
//                        }
//                    }
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
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
