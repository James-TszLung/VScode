/*
求和1+2+...+n的值，用MPI创建k个进程，各进程并行求和（如0号进程求和[1,n/k]，
1号进程求和[n/k+1,2n/k]，依些类推），随后其它进程将求和的结果发送给进程0，
最后由进程0汇总，并将总的求和值输出。
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
int main(int argc, char *argv[]){
	int count=atoi(argv[1]);
	int comm_sz, my_rank, local_n, source;
	double local_int, total_int, start, end;

	MPI_Init(0, 0);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	local_n=count/comm_sz;
	start=local_n*(my_rank)+1;
	end=local_n*(my_rank+1);

	for(double i=start; i<=end; i++){
		local_int+=i;
	}
	printf("Local from %.f to %.f = %f\n", start, end, local_int);
	
	if(my_rank!=0){
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}else{
		printf("-----------------------------------\n");
		total_int=local_int;
		printf("Total from 1 to %d = %f\n", local_n, total_int);
		for(source=1; source<comm_sz; source++){
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int+=local_int;
			printf("Total from 1 to %d = %f\n", local_n*(source+1), total_int);
		}
		sleep(1);
		printf("-----------------------------------\n");
		printf("The result is %f\n", total_int);
	}
	MPI_Finalize();
	return 0;
}
