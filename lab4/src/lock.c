#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;

int main(){
    int n;
    omp_init_lock(&lock);
    int g = 0;
    int threads = 500;
#pragma omp parallel for private(n) shared(g) num_threads(threads)
    for(int i = 0; i < threads; i++){
        n = omp_get_thread_num();
//        printf("I am section %d\n", n);
        if (n % 2 == 0){
            omp_set_lock(&lock);
            g += n;
            omp_unset_lock(&lock);
        }
    }
    omp_destroy_lock(&lock);
    printf("%d\n", g);
}
