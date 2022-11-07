#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int*, unsigned int*, int);
void print_array(int*, int, int);
void shellsort(int*, int, int);

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    for(i=0; i < count; i++) { 
      array[i] = rand(); 
    }
    *random_seed += rand();
}

void print_array(int* array, int count, int t){
    printf("a%d = [", t);
    for(int i = 0; i < count - 1; i++){
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[count-1]);
}

void shellsort(int* array, int count, int threads){
    for(int gap = count/2; gap > 0; gap /= 2){
#pragma omp parallel for shared(gap, count, array) default(none) num_threads(threads)
        for(int i = 0; i < gap; i++){
            for(int part = i + gap; part < count; part += gap){
                for(int j=part; j>i  && array[j-gap] > array[j]; j-=gap){
                    int tmp = array[j];
                    array[j] = array[j-gap];
                    array[j-gap] = tmp;
                }
            }
        }
    }
}
 
    
int main(){
    int n;
    fprintf(stderr, "The number of array to check: ");
    scanf("%d", &n);
    const int count = 10000;
    unsigned int random_seed = 1337;
    const int num_exp = 10;
    const int thread_bound = 10;
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

    print_array(arrays[0][n], count, 0);

    for(threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "Number of threads: %d/%d\n", threads, thread_bound);

        for(int e = 0; e < num_exp; e++){ 
            fprintf(stderr, "Number of experiment: %d/%d\n", e+1, num_exp);
            array = arrays[threads-1][e];
            shellsort(array, count, threads);
        }

        print_array(arrays[threads-1][n], count, threads);
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
