#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
pthread_mutex_t mutex;
long count;   //线程个数
long long terms;    //计算的项数
double sum;     //pi

void *thread_multi(void *para){
    long rank=(long)para;            //线程序号
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
    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *thread_single(void *para){
    double factor=1.0;
    sum=0.0;
    for (size_t i = 0; i < terms; i++, factor=-factor)
    {
        sum+=factor/(2*i+1);
    }
    return 0;
}

int main(int argc, char *argv[]){
    count=atoi(argv[1]);
    terms=atoi(argv[2]);
    pthread_t single, multi[count];
    pthread_mutex_init(&mutex, NULL);
    //创建多线程
    float time=0;
    struct timeval start, finish;
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < count; i++)
    {
        pthread_create(&multi[i], NULL, thread_multi, (void *)i);
    }
    //等待多线程
    for (size_t i = 0; i < count; i++)
    {
        pthread_join(multi[i], NULL);
    }
    gettimeofday(&finish,NULL);
    time=(finish.tv_sec-start.tv_sec)*1000000+(finish.tv_usec-start.tv_usec);
    printf("with n = %lld terms\n", terms);
    printf("Multi-threaded estimate of pi = %16.15f\n", sum*4.0);
    printf("                 Elapsed time = %f seconds\n", time);
    
    //创建单线程
    gettimeofday(&start, NULL);
    pthread_create(&single, NULL, thread_single, NULL);
    //等待单线程
    pthread_join(single, NULL);
    gettimeofday(&finish,NULL);

    time=(finish.tv_sec-start.tv_sec)*1000000+(finish.tv_usec-start.tv_usec);
    double PI=4.0*atan(1.0);
    printf("            Single thread est = %16.15f\n", sum*4.0);
    printf("                 Elapsed time = %f seconds\n", time);
    printf("                           pi = %16.15f\n", PI);

    pthread_mutex_destroy(&mutex);
    return 0;
}