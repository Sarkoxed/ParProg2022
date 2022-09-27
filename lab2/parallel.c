#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 920214; ///< RNG seed
    int threads = 16;

    int* array = 0;                 ///< The array we need to find the max in
    int  index = count + 1;                ///< The index of the element we need
    int i;
    const int chunk = count / threads;
    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)calloc(count, sizeof(int));
   
    #pragma omp parallel for shared(array, count, chunk) default(none) private(i) schedule(dynamic, chunk) num_threads(threads)
    for(i=0; i<count; i++) { 
        array[i] = rand(); 
    }
    srand(time(NULL));
    const int target = array[rand() % count];          ///< Number to look for
    #pragma omp parallel shared(array, count, target, threads) default(none) private(i) num_threads(threads) reduction(min: index)
    {
        i = omp_get_thread_num();
        while(array[i] != target && i < count){
            i += threads;
        }
        index = (i > count)?count:i;
    }

    free(array);
    printf("Found occurence of %d at index %d;\n", target, index);
    return(0);
}
