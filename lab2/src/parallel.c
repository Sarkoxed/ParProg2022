#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void new_array(int* array, unsigned int* random_seed, int count){
    srand(*random_seed);
    int i;
    int threads = 16;
    int chunk = count / threads;
    #pragma omp parallel for shared(array, count, chunk) default(none) private(i) schedule(dynamic, chunk) num_threads(threads)
    for(i=0; i<count; i++) { 
       array[i] = rand(); 
    }
    *random_seed += rand();
}

int main(int argc, char** argv)
{
    const int count = 10000000;
    unsigned int random_seed = 920214;
    const int num_exp = 10;
    const int thread_bound = 10;

    int* array = NULL;
    int  index, i, target, threads;
    const int chunk = count / threads;

    double t1, t2, res;

    array = (int*)calloc(count, sizeof(int));

    for(threads=1; threads <= thread_bound; threads++){
        fprintf(stderr, "curthreads: %d\n", threads);
        res = 0.0;
        for(int j = 0; j < num_exp; j++){
            new_array(array, &random_seed, count);
            target = array[rand() % count];
            index = count + 1; 
            
            t1 = omp_get_wtime();
            #pragma omp parallel for shared(array, count, target) default(none) private(i) num_threads(threads) reduction(min: index)
            for(i=0; i<count; i++){

                if(array[i] == target){
                    index = i;
                }
            }
            t2 = omp_get_wtime();
            res += t2 - t1;
        }
        res /= (double)(num_exp);
        fprintf(stdout, "(%d, %g), ", threads, res);
        random_seed = 920214; ///< RNG seed
    }
    free(array);
    return(0);
}
