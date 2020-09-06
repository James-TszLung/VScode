/*
在MPI中创建4个进程，第1-3号进程向分别向0与进程发送（1，10）、（2，20）、（3，30）、（4，40）。
0号进程使用Tag来区分每个进程发过来的信息，对第一个信息输出屏幕，对第二个信息进行求和（初值为0）
最后将求和值输出屏幕。
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(){
	int comm_sz, my_rank, source;
	int local_x, local_y, total_y;

	MPI_Init(0, 0);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	if(my_rank!=0){
        local_x=my_rank;
        local_y=local_x*10;
		MPI_Send(&local_x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&local_y, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}else{
		printf("-----------------------------------\n");
		for(source=1; source<comm_sz; source++){
			MPI_Recv(&local_x, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("No: %d proccess\n", local_x);
			//MPI_Recv(&local_y, 1, MPI_INT, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //total_y+=local_y;
		}
		printf("-----------------------------------\n");
	}
	
	MPI_Reduce(&local_y, &total_y, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if(my_rank==0){
		printf("The result is %d\n", total_y);
	}
	
	MPI_Finalize();
	return 0;
}
