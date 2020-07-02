#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
pthread_mutex_t mutex;
long count;         //线程个数
long long terms;    //计算的项数
double sum;         //pi

void thread_single(){
    double factor=1.0;
    sum=0.0;
    for (size_t i = 0; i < terms; i++, factor=-factor)
    {
        sum+=factor/(2*i+1);
    }
}

int main(int argc, char *argv[]){
    count=atoi(argv[1]);
    terms=atoi(argv[2]);
    double factor=1.0;
    //使用 OpenMP 指令“parallel for”实现并行, “reduction”归约实现互斥, "private"保证有私有作用域
    #pragma omp parallel for num_threads(count) reduction(+:sum) private(factor)
    {
        for (size_t i = 0; i < terms; i++)
        {
            factor=(i%2==0) ? 1.0 : -1.0;
            sum+=factor/(2*i+1) ;
        }
    }
    printf("with n = %lld terms\n", terms);
    printf("Multi-threaded estimate of pi = %16.15f\n", sum*4.0);
    #pragma omp parallel num_threads(1)
    {
        thread_single();
    }
    double PI=4.0*atan(1.0);
    printf("            Single thread est = %16.15f\n", sum*4.0);
    printf("                           pi = %16.15f\n", PI);

    return 0;
}