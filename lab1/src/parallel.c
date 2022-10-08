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
    int** arrays = NULL;
    int  max;                
    double t1, t2, res;

    arrays = (int**)calloc(num_exp, sizeof(int*));
    for(int t = 0; t < num_exp; t++){
        array = (int*)calloc(count, sizeof(int));
        new_array(array, &random_seed, count);
        arrays[t] = array;
    }
    
    for(int threads = 1; threads <= thread_bound; threads++){
        fprintf(stderr, "curthreads: %d\n", threads);
        res = 0.0;
        for(int j = 0; j < num_exp; j++){
            max = -1;
            t1 = omp_get_wtime();
            #pragma omp parallel num_threads(threads) shared(arrays, count, j) reduction(max: max) default(none)
            {
                #pragma omp for
                for(int i=0; i < count; i++)
                {
                    if(arrays[j][i] > max){ 
                        max = arrays[j][i];
                    }
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
        }
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res); 
    }

    for(int t = 0; t < num_exp; t++){
        free(arrays[t]);
    }
    free(arrays);
    return(0);
}
