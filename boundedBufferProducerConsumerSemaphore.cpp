#include<semaphore.h>
#include<pthread.h>
#include<iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdio.h>
using namespace std;

sem_t exclusiveAccess, Empty, Full;
vector<int> buffer;
void append(int element,int pid)
{
    buffer.push_back(element);
    printf("\t\t\t%d appending %d\n",pid+1,element);
    usleep(10000000);
}
int take(int cid)
{
    int element=buffer.front();
    buffer.erase(buffer.begin());
    printf("\t\t\t\t\t\t%d taking %d\n",cid+1,element);
    usleep(5000000);
    return element;
}
int produceElement(int pid)
{
    int element;
    element=rand()%100;
    printf("%d producing %d\n",pid+1,element);
    usleep(20000000);
    return element;
}
void consumeElement(int element,int cid)
{
    printf("\t\t\t\t\t\t\t\t%d consuming %d\n",cid+1,element);
    usleep(15000000);
}

void *produce(void * pid)
{
    int *Pid=(int *) pid;
    while(true)
    {

        int element=produceElement(*Pid);
        sem_wait(&Full);
        sem_wait(&exclusiveAccess);
        /// critical section start
        append(element,*Pid);
        /// critical section end
        sem_post(&exclusiveAccess);
        sem_post(&Empty);
    }
}
void *consume(void * cid)
{
    int *Cid=(int *) cid;
    while(true)
    {
        sem_wait(&Empty);
        sem_wait(&exclusiveAccess);
        /// critical section start
        int element=take(*Cid);
        /// critical section end
        sem_post(&exclusiveAccess);
        sem_post(&Full);
        consumeElement(element,*Cid);
    }
}
int main()
{
    sem_init(&exclusiveAccess,0,1);
    sem_init(&Empty,0,0);
    sem_init(&Full,0,5);

    pthread_t producer[3], consumer[7];
    for(int k=0;k<3;k++)
    {
         int *i=new int;
        *i=k;
        pthread_create(&producer[k],NULL,&produce,i);
    }

    for(int k=0;k<7;k++)
    {
         int *j=new int;
        *j=k;
        pthread_create(&consumer[k],NULL,&consume,j);
    }

    for(int k=0;k<3;k++)
        pthread_join(producer[k],NULL);

    for(int k=0;k<7;k++)
        pthread_join(consumer[k],NULL);

    return 0;
}

