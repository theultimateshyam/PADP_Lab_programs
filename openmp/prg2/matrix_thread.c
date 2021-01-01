#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
void checkCorrectness(int **arr3, int **arr4,int p,int r,int c){
	int l,k;
	for(l=0;l<r;l++){
		for(k=0;k<c;k++){
			if(arr3[l][k] != arr4[l][k]){
				printf("NOT CORRECT FOR THREADS %d \t\n", p);
				return;
			}
		}
	}
	return;
}
int main(){

	int it=1,num=10;
	printf("The execution times are\nSize\t\t1\t\t\t2\t\t\t4\t\t\t8\n");
	while(it<=4){
		int r =num*10,c = num*10, i, j, sum =0, k;
		// allocate array1
		int **arr1 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr1[i] = (int *)malloc(c * sizeof(int));
		
		//allocate  array2
		int **arr2 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr2[i] = (int *)malloc(c * sizeof(int));
		
		//allocate array3
		int **arr3 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr3[i] = (int *)malloc(c * sizeof(int));

		//allocate another array for parallel
		int **arr4 = (int **)malloc(r *sizeof(int *));
			for (i=0;i<r;i++)
				arr4[i] = (int *)malloc(c * sizeof(int));

		for(i = 0;i < r; i++)
			for(j = 0;j < c; j++)
				arr1[i][j] = rand()/r;

		for(i = 0;i < r; i++)
			for(j = 0;j < c; j++)
				arr2[i][j] = rand()/r;

		//serial or non-parallel execution
		double x = omp_get_wtime();

		for(i = 0;i < r; i++)
			for(j = 0;j < c; j++){
				arr3[i][j]=0;
				for(k = 0;k < r; k++)
					arr3[i][j] += arr1[i][k] * arr2[k][j];
			}


		double y = omp_get_wtime();
		
		//parallised execution
		printf("%d\t\t",r);
		printf("%lf\t\t", y-x);
		for(int p=2;p<=8;p=p*2)
		{
			double x = omp_get_wtime(); //++++++++++++++++++++++++why declare x here again
			omp_set_num_threads(p);
			#pragma omp parallel for private(j,k) //+++++++++++++++++only j,k?
			for(i = 0;i < r; i++)
				for(j = 0;j < c; j++)
				{
					arr4[i][j]=0;
					for(k = 0;k < r; k++)
						arr4[i][j] += arr1[i][k] * arr2[k][j];
				}
			double y = omp_get_wtime();
			
			checkCorrectness(arr3,arr4,p,r,c);
			printf("%lf\t\t", y-x);
		}
		printf("\n");
		it++;
		num*=2;
	}
	return 0;
}
