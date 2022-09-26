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
    int  max;                
    double t1, t2, res;

    array = (int*)calloc(count, sizeof(int));
    
    for(int threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "curthreads: %d\n", threads);
        res = 0.0;
        for(int i = 0; i < num_exp; i++){
            new_array(array, &random_seed, count);
            max = -1;
            t1 = omp_get_wtime();
            #pragma omp parallel num_threads(threads) shared(array, count) reduction(max: max) default(none)
            {
                #pragma omp for
                for(int i=0; i < count; i++)
                {
                    if(array[i] > max){ 
                        max = array[i];
                    }
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
        }
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res); 
        random_seed = 920215; 
    }
    free(array);
    return(0);
}
