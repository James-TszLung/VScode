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

void thread_multi(){
    int rank=omp_get_thread_num();  //线程序号
    int total=omp_get_num_threads();//线程个数
    double factor;
    double my_sum;
    long long my_n=terms/count; //每个线程计算my_n个项
    long long first=my_n*rank;  //第一项位置i
    long long last=first+my_n;  //最后一项位置i
    if (first%2==0) //位置为偶数 因数为+
    {
        factor=1.0;
    }else           //位置为奇数 因数为-
    {
        factor=-1.0;
    }
    for (size_t i = first; i < last; i++, factor=-factor)
    {
        my_sum+=factor/(2*i+1) ;
    }
    //互斥
    // pthread_mutex_lock(&mutex);
    #pragma omp critical
    sum+=my_sum;
    // pthread_mutex_unlock(&mutex);

}

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
    #pragma omp parallel num_threads(count)
    {
        thread_multi();
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

    pthread_mutex_destroy(&mutex);
    return 0;
}