#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    int threads = 16;
    #pragma omp parallel for shared(array, count, chunk) default(none) private(i) num_threads(threads)
    for(i=0; i<count; i++) { 
       array[i] = rand(); 
    }
    *random_seed += rand();
}

int main(){
    const int count = 10000000;
    unsigned int random_seed = 1337;
    const int num_exp = 1;
    
    //int**arrays = NULL;
    int* array = NULL;
    array = (int*)calloc(count, sizeof(int));

    

    free(array);
    return 0;
}
