#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
pthread_mutex_t mutex;  //上桥互斥量
pthread_mutex_t left;   //左边上桥信号量
pthread_mutex_t right;  //右边上桥信号量
int leftcar=0;          //左边已上桥车辆
int rightcar=0;         //右边已上桥车辆

void *fun_left(void *para){
    int n=(long)para+1;
    long Tid=(long)pthread_self();
    srand(Tid*n);
    unsigned int t=rand()%5+1;
    //到达桥头时间随机
    sleep(t);
    if (leftcar>2)
    {
        printf("拥塞避免\n");
        while(leftcar>0);
    }
    
    pthread_mutex_lock(&left);
    leftcar++;
    if (leftcar==1)
    {
        pthread_mutex_lock(&mutex); //左边首辆车上桥，等待过桥互斥量
    }
    pthread_mutex_unlock(&left);

    printf("左边 %d号车上桥 %d秒\n", n, t);
    sleep(t);       //过桥时间随机
    printf("\t\t\t左边 %d号车下桥\n", n);

    pthread_mutex_lock(&left);
    leftcar--;
    if (leftcar==0) //若桥上没车，允许释放过桥信号量
    {
        pthread_mutex_unlock(&mutex);
        printf("\n");
    }
    pthread_mutex_unlock(&left);

    return 0;
}

void *fun_right(void *para){
    int n=(long)para+1;
    long Tid=(long)pthread_self();
    srand(Tid*n);
    unsigned int t=rand()%5+1;
    //到达桥头时间随机
    sleep(t);
    if (rightcar>2)
    {
        printf("拥塞避免\n");
        while(rightcar>0);
    }
    
    pthread_mutex_lock(&right);
    rightcar++;
    if (rightcar==1)
    {
        pthread_mutex_lock(&mutex); //右边首辆车上桥，等待过桥互斥量
    }
    pthread_mutex_unlock(&right);

    printf("右边 %d号车上桥 %d秒\n", n, t);
    sleep(t);       //过桥时间随机
    printf("\t\t\t右边 %d号车 下桥\n", n); 

    pthread_mutex_lock(&right);
    rightcar--;
    if (rightcar==0) //若桥上没车，允许释放过桥信号量
    {
        pthread_mutex_unlock(&mutex);
        printf("\n");
    }
    pthread_mutex_unlock(&right);

    return 0;
}

int main(int argc, char * argv[]){
    int leftnum=atoi(argv[1]);      //左边车辆数
    int rightnum=atoi(argv[2]);     //右边车辆数
    pthread_t ltid[leftnum], rtid[rightnum];
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&left, NULL);
    pthread_mutex_init(&right, NULL);

    //创建线程
    for (size_t i = 0; i < leftnum; i++)
    {
        pthread_create(&ltid[i], NULL, fun_left, (void *)i);
    }
    for (size_t i = 0; i < rightnum; i++)
    {
        pthread_create(&rtid[i], NULL, fun_right, (void *)i);
    }
    //等待同步
    for (size_t i = 0; i < leftnum; i++)
    {
        pthread_join(ltid[i], NULL);
    }
    for (size_t i = 0; i < leftnum; i++)
    {
        pthread_join(rtid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&left);
    pthread_mutex_destroy(&right);
    return 0;
}
