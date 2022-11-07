#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;

int main(){
    int n;
    omp_init_lock(&lock);
#pragma omp parallel private(n)
    {
        n = omp_get_thread_num();
        omp_set_lock(&lock);
        printf("Closed section, thread %d\n", n);
        sleep(5);
        printf("Closed section end, thread %d\n", n);
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
}
