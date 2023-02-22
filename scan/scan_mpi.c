#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

#define size 32768 //262144 32768 4096 512 64 8

#define power 15//18 15 12 9 6 3

int main(int argc, char *argv[])
{

    int initial[size];
    int arr1[size/2];
    int arr2[size/2];
    int out[size];
    int valid = 0;
    FILE    *timer;

    int num_procs, myrank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(myrank == 0){

        timer = fopen("Times.txt", "a");
        fprintf(timer, "MPI implementation:\n");

        for(int i = 0; i<size;i++ ){
            initial[i] = i +1;
        }
        //initial[0] = 10;

       // srand(time(NULL));
        //for(int i = 0; i < size; i++){
           // x = rand() % 10;
            //initial[i] = x;
        //}

        for(int i = 0;i < size; i++){
           // printf("%d\n",initial[i]);
        }

        memcpy(arr1, initial, (size/2) * sizeof(int));
        memcpy(arr2, initial + (size/2), (size/2) * sizeof(int));

        MPI_Send(arr2,(size/2),MPI_INT,1,0,MPI_COMM_WORLD);

        //UP SWEEP ================================================

        int k = 0;
        int x = 0;

        for(int i = 1; i < power+1; i++){
            for(int j = 0; j < size/2; j++){
                j+= (pow(2, i) -1);
                x = pow(2, k);
                arr1[j] = arr1[j-x] + arr1[j];
            }
            k++;
        }

        //DO FIRST STEP OF DOWN SWEEP ==================================
        MPI_Recv (arr2,(size/2),MPI_INT,1,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int temp = arr2[(size/2) - 1];
        arr2[(size/2) - 1] = arr1[(size/2) - 1] + temp;
        arr1[(size/2) - 1] = temp;

        MPI_Send(arr2,(size/2),MPI_INT,1,2,MPI_COMM_WORLD);

        //REGULAR DOWN SWEEP ============================================

        //int x;
        clock_t t;
        t = clock();
        for(int i = power - 1; i > -1; i--){ //i goes from 2 -> 0
            for(int j = (size/2)-1; j > 0; j--){ //j shows the elemnt in each step
                x = pow(2, i);
                temp = arr1[j - x];
                arr1[j - x] = arr1[j];
                arr1[j] = arr1[j] + temp;
                //printf("%d\n", j);
                j -= pow(2, i+1) -1; // j = 5

            }
        }
        t = clock()-t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;

        MPI_Recv (arr2,(size/2),MPI_INT,1,3,MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        for(int i = 0; i < size/2; i++){
            out[i] = arr1[i];
        }
        int z = 0;
        for(int i = size/2; i < size; i++){
            out[i] = arr2[z];
            z++;
        }

        //PRINTING ====================================================
        for(int i = 0; i < size-1; i++){
            if(out[i] > out[i+1]){
                valid = 1;
            }
        }
        if(valid == 0){
            printf("MPI Valid \n");
        }
        printf( "Elapsed time is %f\n", time_taken );
        fprintf(timer, "Array Size: 2^%d | Time Taken: %f s\n", 15, time_taken);
        fclose(timer);


    }
    else if(myrank == 1){
        //upSweep(arr2);
        MPI_Recv (arr2,(size/2),MPI_INT,0,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        //UP SWEEP ================================================

        int k = 0;
        int x = 0;

        for(int i = 1; i < power+1; i++){
            for(int j = 0; j < size/2; j++){
                j+= (pow(2, i) -1);
                x = pow(2, k);
                arr2[j] = arr2[j-x] + arr2[j];
            }
            k++;
        }

        //DO FIRST STEP OF DOWN SWEEP ==================================


        arr2[(size/2) - 1] = 0;

        MPI_Send(arr2,(size/2),MPI_INT,0,1,MPI_COMM_WORLD);

        MPI_Recv (arr2,(size/2),MPI_INT,0,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //REGULAR DOWN SWEEP ============================================
        int temp = 0;
        for(int i = power - 1; i > 0; i--){ //i goes from 2 -> 0
            for(int j = (size/2)-1; j > 0; j--){ //j shows the elemnt in each step
                x = pow(2, i-1);
               // printf("j:%d x: %d\n", j, x);
                temp = arr2[j - x];
                arr2[j - x] = arr2[j];
                arr2[j] = arr2[j] + temp;
                //printf("%d\n", temp);
                j -= pow(2, i) -1; // j = 5

            }
        }


        MPI_Send(arr2,(size/2),MPI_INT,0,3,MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}







































