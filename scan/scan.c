#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

//#define size 262144

void scan(int in[], int out[], int size){
    int i = 0;
    out[0] = in[0];

    for(i=1;i<size;i++){
        out[i] = (in[i]+out[(i-1)]);
    }
}

int main()
{
    fclose(fopen("Times.txt", "w"));
    fclose(fopen("Results.txt", "w"));

    FILE    *timer;
    FILE    *output;

    int valid = 0;

    timer = fopen("Times.txt", "a");
    output = fopen("Results.txt", "a");
    fprintf(timer, "Serial implementation:\n");
    fprintf(output, "Serial array:\n (");

    for(int p = 3; p < 19; p += 3){

        clock_t t;
        t = clock();
        int size = pow(2, p);
        int x;
        int *initial = (int *) malloc(sizeof(int)*size);
        int *sumList = (int *) malloc(sizeof(int)*size);

        srand(time(NULL));
        for(int i = 0; i < size; i++){
            x = rand() % 100;
            initial[i] = x;
            if(p==3){
                initial[i] = i;
            }

        }

        scan(initial, sumList, size);

        t = clock()-t;

        double time_taken = ((double)t)/CLOCKS_PER_SEC;

        //printing

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
        printf("Serial is Valid and timed\n");
    }
    fprintf(output, ")\n");
    fclose(timer);
    fclose(output);

    return 0;
}
