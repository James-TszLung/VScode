/*任务：完成2个向量求和：Z=X+Y X=(1,2,3,4,5,6,7,8); Y=(0,1,2,3,4,5,6,7)
创建4个MPI进程
0号进程从控制台读入向量维数n(=8)，二个向量的值
0号进程信息发送给其它进程（利用广播函数MPI_Bcast)
各进程并行求和
0号进程汇总求和结果，并输出
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int main(){
	int n, comm_sz, my_rank;
	int *x, *y, *z, *local_z, local_n;

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
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_n=n/comm_sz;

    if (my_rank!=0)
    {
        x=(int*)malloc(n * (sizeof(int)));
        y=(int*)malloc(n * (sizeof(int)));
    }
    local_z=malloc(n*(sizeof(int)));
    memset(local_z, 0, n*(sizeof(int)));

    MPI_Bcast(x, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, n, MPI_INT, 0, MPI_COMM_WORLD);
	
    for (int i = my_rank*local_n; i < (my_rank+1)*local_n; i++)
    {
        printf("x %d, y %d, ", x[i], y[i]);
        local_z[i]=x[i]+y[i];
        printf("z %d\n", z[i]); fflush(stdout);
    }
    if (my_rank==0)
    {
        MPI_Reduce(local_z, z, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }else
    {
        MPI_Reduce(local_z, NULL, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    
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


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int main(){
	int n, comm_sz, my_rank;
	MPI_Init(0,0);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int max=10;
	int x[max], y[max], z[max], sum[max];
    memset(z, 0, max);//必要
    memset(sum, 0, max);
    if (my_rank==0)
    {
        printf("输入向量维数："); fflush(stdout);
        scanf("%d", &n);
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
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, n, MPI_INT, 0, MPI_COMM_WORLD);
	
    for (int i = (my_rank*n)/4; i < (my_rank+1)*n/4; i++)
    {
        printf("x %d, y %d, ", x[i], y[i]);
        z[i]=x[i]+y[i];
        printf("z %d\n", z[i]); fflush(stdout);
    }

    MPI_Reduce(z, sum, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank==0)
    {
        printf("\nsum:");
        for (int i = 0; i < n; i++)
        {
            printf(" %d", sum[i]); 
        }
    }

	MPI_Finalize();
    return 0;
}

*/