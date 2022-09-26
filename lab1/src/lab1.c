#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     
    const int threads = 16;         
    const int random_seed = 920215; 

    int* array = NULL;             
    int  max   = -1;              

    /* Initialize the RNG */
    srand(random_seed);

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n======\n", _OPENMP);

    /* Generate the random array */
    array = (int*)calloc(count, sizeof(int));
    for(int i=0; i < count; i++){
        array[i] = rand(); 
    }

    /* Find the maximal element */
    #pragma omp parallel num_threads(threads) shared(array, count) reduction(max: max) default(none)
    {
        #pragma omp for
        for(int i=0; i < count; i++)
        {
            if(array[i] > max){ 
                max = array[i];
            }
        }
        printf("-- My lmax is: %d;\n", max);
    }

    printf("======\nMax is: %d;\n", max);
    return(0);
}
