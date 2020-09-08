/*条件变量实现生产者消费者问题
1.多个生产者线程，每生产一个产品(数字)，对共享变量counter加1，并向被条件变量cond阻塞的线程发送唤醒信号。
    1)线程数通过命令行参数输入
    2)指定数字缓冲区大小为10。
2.一个消费者线程，如果counter==0，则在条件变量cond阻塞；否则，取出产品(数字)，并对counter减1。
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
int counter=0;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
pthread_cond_t cond;

struct queue{
    int date[10];   //指定数字缓冲区大小为10
    int front;
    int rear;
}que;

void *pfun(void *para){
    int rank=(long)para;
    do
    {
        long Tid=(long)pthread_self();
        srand((unsigned)time(0)-Tid);
        sleep(1);
        //每生产一个产品(数字)，对共享变量counter加1，并向被条件变量cond阻塞的线程发送唤醒信号
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        que.date[que.rear]=rand()%100;
        printf("生产者 %d 生产产品 %d\n", rank, que.date[que.rear]);
        que.rear=(que.rear+1)%10;
        counter++;
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    } while (1);

}
void *cfun(void *para){
    do
    {
        sleep(1);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        //如果counter==0，则在条件变量cond阻塞；否则，取出产品(数字)，并对counter减1
        if (counter==0)
        {
            printf("阻塞线程");
            while(pthread_cond_wait(&cond, &mutex)!=0);
        }else
        {
            printf("消费者   取出产品 %d\n", que.date[que.front]);
            que.date[que.front]=-1;
            que.front=(que.front+1)%10;
            counter--;
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    } while (1);
}
int main(int argc, char *argv[]){
    //线程数通过命令行参数输入
    int n=atoi(argv[1]);
    pthread_t producer[n], consumer;
    que.front=que.rear=0;
    for (size_t i = 0; i < 10; i++)
    {
        que.date[i]=-1;
    }
    
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);
    for (size_t i = 0; i < n; i++){
        pthread_create(&producer[i], NULL, pfun, (void *)i);
    }
    
    pthread_create(&consumer, NULL, cfun, NULL);

    for (size_t i = 0; i < n; i++){
        pthread_join(producer[i], NULL);
    }
    pthread_join(consumer, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;

}