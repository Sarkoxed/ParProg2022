#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    int chunk = count / 16;
    #pragma omp parallel for shared(array, count, chunk) default(none) private(i) schedule(dynamic, chunk) num_threads(16)
    for(i=0; i<count; i++) { 
       array[i] = rand(); 
    }
}

void output_array(int* array, int count, int num){
    printf("arr%d = [", num);
    for(int i = 0; i < count - 1; i++){
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[count-1]);
}


int main(int argc, char** argv)
{
    const int count = 1000000;     ///< Number of array elements
    unsigned int random_seed = 920214; ///< RNG seed
    int threads;
    int* array = NULL;                 ///< The array we need to find the max in

    array = (int*)calloc(count, sizeof(int));
    new_array(array, &random_seed, count);
    output_array(array, count, 1);
    new_array(array, &random_seed, count);
    output_array(array, count, 2);
    free(array);
    return 0;
}

