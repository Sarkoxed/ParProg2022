#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int*, unsigned int*, int);

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    for(i=0; i < count; i++) { 
       array[i] = rand(); 
    }
    *random_seed += rand();
}

double shellsort(int* array, int count, int* comp){
    double t1, t2;
    *comp = 0;
    t1 = omp_get_wtime();
    for(int gap = count/2; gap > 0; gap /= 2){
        for(int i = gap; i < count; i++){
            for(int j=i; j>=gap  && array[j-gap] > array[j]; j-=gap){
                *comp = *comp + 1;
                int tmp = array[j];
                array[j] = array[j-gap];
                array[j-gap] = tmp;
            }
        }
    }
    t2 = omp_get_wtime();
    return t2 - t1;
}
 

int main(){
    int count = 1000000;
    unsigned int random_seed = 1337;
    const int num_exp = 10;
    
    int**arrays = NULL;
    int* array = NULL;

    arrays = (int**)calloc(num_exp, sizeof(int*));
    for(int t = 0; t < num_exp; t++){
        arrays[t] = (int*)calloc(count, sizeof(int));
        new_array(arrays[t], &random_seed, count);
    }

    double res = 0.0;
    int comp, res1 = 0;

    for(int e = 0; e < num_exp; e++){ 
        fprintf(stderr, "Number of experiment: %d/%d\n", e+1, num_exp);
        array = arrays[e];
        res += shellsort(array, count, &comp);
        res1 += comp;
    }
//    printf("Num of iterations: %d, count: %d\n", r, count);
    res /= (double)(num_exp);
    fprintf(stdout, "%g\n", res); 
//    fprintf(stdout, "%d\n", res1 / num_exp); 


    for(int t = 0; t < num_exp; t++){
        free(arrays[t]);
    }
    free(arrays);
    return 0;
}
