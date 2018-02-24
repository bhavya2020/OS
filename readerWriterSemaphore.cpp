#include<semaphore.h>
#include<pthread.h>
#include<iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdio.h>
using namespace std;

int rCount=0,wCount=0;
sem_t wSem,rSem,rCountSem,wCountSem,manyReadersLock,manyWritersLock;
vector<int> file;
void writeUnit(int wid)
{
    int element;
    element=rand()%100;
    file.push_back(element);
    printf("%d writing %d \n",wid+1,element);
    usleep(1000000);
}
void readUnit(int rid)
{
  usleep((rid+1)*1000000);
  printf("\t\t\t\t\t\t%d reading ",rid+1);
  for(int i=0;i<file.size();i++)
  {
      printf("%d, ",file[i]);
  }
  printf("\n");

}

void *write(void * wid)
{
    while(true)
    {
        int *Wid=(int *) wid;
        usleep(1000000);
        sem_wait(&wCountSem);
        wCount++;

        if(wCount==1)
            sem_wait(&rSem);
        sem_post(&wCountSem);


        sem_wait(&wSem);
        writeUnit(*Wid);
        sem_post(&wSem);

        sem_wait(&wCountSem);
        wCount--;
        if(wCount==0)
        {
          sem_post(&rSem);
        }
        usleep(100000);
        sem_post(&wCountSem);

    }
}
void *read(void * rid)
{
    int *Rid=(int *) rid;
    while(true)
    {
        sem_wait(&manyReadersLock);
        usleep(1000000);
        sem_wait(&rSem);

        sem_wait(&rCountSem);
        rCount++;
        if(rCount==1)
            sem_wait(&wSem);
        sem_post(&rCountSem);


        sem_post(&rSem);
        sem_post(&manyReadersLock);

        readUnit(*Rid);

        sem_wait(&rCountSem);
        rCount--;
        if(rCount==0)
            sem_post(&wSem);
        sem_post(&rCountSem);
    }
}
int main()
{
    sem_init(&wSem,0,1);
    sem_init(&rSem,0,1);
    sem_init(&rCountSem,0,1);
    sem_init(&wCountSem,0,1);
    sem_init(&manyReadersLock,0,3);
    sem_init(&manyWritersLock,0,2);

    pthread_t writer[2], reader[10];
    for(int k=0;k<2;k++)
    {
         int *i=new int;
        *i=k;
        pthread_create(&writer[k],NULL,&write,i);
    }
    for(int k=0;k<10;k++)
    {
         int *j=new int;
        *j=k;
        pthread_create(&reader[k],NULL,&read,j);
    }


    for(int k=0;k<2;k++)
        pthread_join(writer[k],NULL);

    for(int k=0;k<10;k++)
        pthread_join(reader[k],NULL);

    return 0;
}


