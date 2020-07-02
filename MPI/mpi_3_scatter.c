#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int main(){
	int n, comm_sz, my_rank, local_n;
	int *x, *local_x, *local_y, *y, *z, *local_z;

	MPI_Init(0,0);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank==0)
    {
        printf("输入向量维数："); fflush(stdout);
        scanf("%d", &n);
        x=(int*)malloc(n * (sizeof(int)));
        y=(int*)malloc(n * (sizeof(int)));
        z=(int*)malloc(n * (sizeof(int)));
        printf("输入x向量："); fflush(stdout);
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &x[i]);
        }
        printf("输入y向量："); fflush(stdout);
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &y[i]);
        }
    }
    //广播
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n=n/comm_sz;
    local_x=(int*)malloc(n * (sizeof(int)));
    local_y=(int*)malloc(n * (sizeof(int)));
    local_z=(int*)malloc(n * (sizeof(int)));
    //散射
    MPI_Scatter(x, local_n, MPI_INT, local_x, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, local_n, MPI_INT, local_y, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = my_rank*local_n; i < (my_rank+1)*local_n; i++)
    {
        printf("x %d, y %d, ", x[i], y[i]);
        local_z[i]=local_x[i]+local_y[i];
        printf("z %d\n", z[i]); fflush(stdout);
    }
    //聚集
    MPI_Gather(local_z, local_n, MPI_INT, z, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (my_rank==0)
    {
        printf("\nsum:");
        for (int i = 0; i < n; i++)
        {
            printf(" %d", z[i]); 
        }
    }

	MPI_Finalize();
    return 0;
}