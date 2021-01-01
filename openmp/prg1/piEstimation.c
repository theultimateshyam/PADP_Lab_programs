#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{   srand(8861);
    long n , i, count = 0;
    double z, pi, x, y;
    int num_threads;
    srand(omp_get_wtime());
    double finishtime ;
    double starttime = omp_get_wtime();
    printf("Num of threads \tNum of pts   Value of Pi\n");
    for (n=1000; n<=10000000; n= n*10){

        for (num_threads = 1; num_threads <= 8; num_threads= num_threads*2){
            printf("%d\t\t",num_threads);
            count = 0;
            #pragma omp private(x,y,z) FOR parallel num_threads(num_threads)
                for (i = 0; i < n; i++)
                {
                    x = (double)rand() / RAND_MAX;
                    y = (double)rand() / RAND_MAX;
                    z = x * x + y * y;
                    if (z <= 1)
                        count++;
                }

                pi = (double)count / n * 4;
                printf("%d\t%f \n", n, pi);
        }
    }
    finishtime = omp_get_wtime();
    printf("\nExecution time is : %f", (double)(finishtime - starttime));

    return 0;
}