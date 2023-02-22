#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

//#define size 262144

void scan(int in[], int out[], int size){

    int nthreads, *temp, *x = out;

    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int i;
        #pragma omp single
        {
            nthreads = omp_get_num_threads();
            temp = malloc(sizeof(int)*nthreads+1);
            temp[0] = 0;
        }
        int tid = omp_get_thread_num();
        int sum = 0;

        #pragma omp for schedule(static)
        for(i = 0; i < size; i++){
            sum += in[i];
            x[i] = sum;
        }
        temp[tid+1] = sum;

        #pragma omp barrier
        int offset = 0;
        for(i = 0; i< tid+1; i++){
            offset += temp[i];
        }

        #pragma omp for schedule(static)
        for(i = 0; i < size; i++){
            x[i] += offset;
        }

        //delete[] temp;

    }
    free(temp);
}

int main()
{
    FILE    *timer;
    FILE    *output;
    int valid = 0;

    timer = fopen("Times.txt", "a");
    output = fopen("Results.txt", "a");
    fprintf(timer, "OpenMP implementation:\n");
    fprintf(output, "OpenMP array:\n (");


    for(int p = 3; p < 19; p += 3){
        int size = pow(2, p);
        int x;
        int *initial = (int *) malloc(sizeof(int)*size);
        int *sumList = (int *) malloc(sizeof(int)*size);

        srand(time(NULL));
        for(int i = 0; i < size; i++){
            x = rand() % 100;
            initial[i] = x;
            if(p == 3){
                initial[i] = i;
            }
        }

        clock_t t;
        t = clock();

        scan(initial, sumList, size);

        t = clock()-t;

        double time_taken = ((double)t)/CLOCKS_PER_SEC;

        //Printing

        fprintf(timer, "Array Size: 2^%d | Time Taken: %f s\n", p, time_taken);

        for(int i = 0; i < size -1; i++){
            if(sumList[i] > sumList[i+1]){
                //printf("i:%d ---- %d > %d\n", i, sumList[i], sumList[i+1]);
                valid = 1;
            }
        }
        for(int i = 0; i < size; i++){
            if(p == 3){
                fprintf(output, "%d, ", sumList[i]);
            }
        }

        free(initial);
        free(sumList);
     }
    if(valid == 0){
        printf("OpenMP is Valid and timed\n");
    }
    fprintf(output, ")\n");
    fclose(timer);
    fclose(output);

    return 0;
}

