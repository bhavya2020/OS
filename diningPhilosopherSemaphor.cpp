#include<semaphore.h>
#include<pthread.h>
#include<iostream>
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdio.h>
using namespace std;
sem_t forks[5];
sem_t room;


void think(int pid)
{
    printf("%d thinking\n",pid+1);
    usleep(7000000);
}
void eat(int pid)
{
    printf("\t\t\t\t\t%d eating\n\n",pid+1);
    usleep(10000000);
}
void *philosopher(void *Pid)
{
    int *pid=(int *) Pid;
    //cout<<*pid<<" t\n";
    while(true)
    {
        //cout<<*pid<<" t\n";
        think(*pid);

        sem_wait(&room);
        sem_wait(&forks[*pid]);
        sem_wait(&forks[(*pid+1)%5]);
        /// critical section start
        eat(*pid);
        /// critical section end
        sem_post(&forks[(*pid+1)%5]);
        sem_post(&forks[*pid]);
        sem_post(&room);
    }

}
int main()
{
      sem_init(&room,0,4);
      for(int i=0;i<5;i++)
      {
         sem_init(&forks[i], 0, 1);
      }
    pthread_t threads[5];

    for(int i=0;i<5;i++)
    {
        int *j=new int;
        *j=i;
        //cout<<*j<<"\n";
        pthread_create(&threads[i],NULL,&philosopher,j);

    }
    for(int i=0;i<5;i++)
    {
        pthread_join( threads[i], NULL);
    }

    return 0;
}
