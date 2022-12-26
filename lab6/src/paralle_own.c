#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void new_array(int*, unsigned int*, int);

void new_array(int* array, unsigned int* random_seed, int count){
    srand(time(NULL));
    int i;
    for(i=0; i < count; i++) { 
      array[i] = rand() % 100; 
    }
    *random_seed += rand();
}

void subsort(int* array, int count){
    for(int part = 1; part < count; part++){
        for(int j=part; j > 0  && array[j-1] > array[j]; j--){
            int tmp = array[j];
            array[j] = array[j-1];
            array[j-1] = tmp;
        }
    }
}

int main(int argc, char** argv){
    const int count = 100;
    unsigned int random_seed = 1337;
    int num_exp = 20;
    double t1, t2;

    int ret = MPI_Init(&argc, &argv);
    int size, rank;
    int* array = (int*)calloc(count, sizeof(int));

    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        new_array(array, &random_seed, count);
    }

    int n = count / size;

    if(rank == 0){
        t1 = MPI_Wtime();
        for(int i = 0; i < count; i++){
            printf("%d ", array[i]);
        }
        printf("\n\n");

        for(int gap = count/2; gap > 1; gap /= 2){
            int partlen = (count + gap - 1) / gap;
            for(int i = 0; i < gap; i+=(size-1)){
                for(int j = i; j < i + size - 1 && j < count; j++){
                    int* part = (int*)calloc(partlen, sizeof(int));
                    int len = 0;
                    for(int t = j; t < count; t+= gap){
                        part[t/gap] = array[t];
                        len++;
                    }
                    MPI_Send(&len, 1, MPI_INT, i % (size - 1) + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(part, len, MPI_INT, i % (size - 1) + 1, 0, MPI_COMM_WORLD);
                    free(part);
                }
                for(int j = i; j < i + size - 1 && j < count; j++){
                    int len;
                    MPI_Recv(&len, 1, MPI_INT, i % (size - 1) + 1, 1, MPI_COMM_WORLD, &status);
                    int* part = (int*)calloc(len, sizeof(int));
                    MPI_Recv(part, len, MPI_INT, i % (size - 1) + 1, 1, MPI_COMM_WORLD, &status);
                    for(int t = i; t < count; t+= gap){
                        array[t] = part[t/gap];
                    }
                    free(part);
                }
            }
//            for(int i = 0; i < count; i++){
//                printf("%d ", array[i]);
//            }
//            printf("\n");
        }

        for(int i = 0; i < size; i++){
            int x = 0;
            MPI_Send(&x, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD);
        }

        t2 = MPI_Wtime();
        printf("ANS\n");
        for(int i = 0; i < count; i++){
            printf("%d ", array[i]);
        }
        printf("\n\n");

        fprintf(stdout, "(%d, %g), ", size, t2 - t1);
        free(array);
    }
    else{
        int len = 1;
        printf("I am here %d\n", rank);
        while(len != 0){
            MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if(len != 0){
                int* part = (int*)calloc(len, sizeof(int));
                MPI_Recv(part, len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                printf("%d______________\n", rank);
                for(int i = 0; i < count; i++){
                    printf("%d ", array[i]);
                }
                printf("\n");
 
                subsort(part, len);
                MPI_Send(&len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                MPI_Send(part, len, MPI_INT, 0, 1, MPI_COMM_WORLD);
                free(part);
            }
        }
    }
    MPI_Finalize();
    return 0;
}
