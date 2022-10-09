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

void print_array(int* array, int count, int t){
    printf("a%d = [", t);
    for(int i = 0; i < count - 1; i++){
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[count-1]);
}
    
int main(){
//    int n;
//    scanf("%d", &n);
    const int count = 100000;
    unsigned int random_seed = 1337;
    const int num_exp = 2;
    const int thread_bound = 8;
    int threads;
    
    int**arrays = NULL;
    int* array = NULL;

    arrays = (int**)calloc(num_exp, sizeof(int*));
    for(int t = 0; t < num_exp; t++){
        arrays[t] = (int*)calloc(count, sizeof(int));
        new_array(arrays[t], &random_seed, count);
    }

//    print_array(arrays[n], count, 0);
    double t1, t2, res = 0.0;
    
    for(threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "Number of threads: %d/%d\n", threads, thread_bound);
        res = 0.0;
        for(int e = 0; e < num_exp; e++){ 
            fprintf(stderr, "Number of experiment: %d/%d\n", e+1, num_exp);
            array = arrays[e];
            
            t1 = omp_get_wtime();
            for(int gap = count/2; gap > 0; gap /= 2){
                for(int i = gap; i < count; i+=gap){
                    int tmax = (i + gap > count)?count:i+gap;
#pragma omp parallel for shared(i, gap, array, tmax) default(none) num_threads(threads)
                    for(int k = i; k < tmax; k++){
                        for(int j=k; j>=gap  && array[j-gap] > array[j]; j-=gap){
                            int tmp = array[j];
                            array[j] = array[j-gap];
                            array[j-gap] = tmp;
                        }
                    }
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
        }
 //       print_array(arrays[n], count, threads);
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res); 
        
        // "unsorting" arrays
        random_seed = 1337;
        for(int t = 0; t < num_exp; t++){
            new_array(arrays[t], &random_seed, count);
        }

    }
    
    for(int t = 0; t < num_exp; t++){
        free(arrays[t]);
    }
    free(arrays);
    return 0;
}
