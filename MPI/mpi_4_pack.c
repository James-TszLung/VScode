#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int main(){
	int comm_sz, my_rank;
    int n;
    long n_addr, a_addr, b_addr;
	double a, b;
    MPI_Datatype new_type;
	MPI_Init(0,0);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //描述每个对象有多少个元素
    int lengths[]={1,1,1};
    //获取变量之间的距离
    MPI_Get_address(&n, &n_addr);
    MPI_Get_address(&a, &a_addr);
    MPI_Get_address(&b, &b_addr);
    //对象之间的距离
    long disp[]={0, a_addr-n_addr, b_addr-n_addr};
    //数据类型
    MPI_Datatype types[]={MPI_INT, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Type_create_struct(3,lengths, disp, types, &new_type);
    //提交
    MPI_Type_commit(&new_type);

    if (my_rank==0)
    {
        printf("输入整数、浮点数、浮点数：");fflush(stdout);
        scanf("%d%lf%lf", &n, &a, &b);
        MPI_Send(&n, 1, new_type, 1, 0, MPI_COMM_WORLD);
    }
    
    if (my_rank==1)
    {
        MPI_Recv(&n, 1, new_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("n=%d, a=%lf, b=%lf\n", n, a, b);
    }

	MPI_Finalize();
    return 0;
}