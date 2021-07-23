#include "msprog4.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define TRUE 1 // set value of true to true
#define MaxItems 5
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int in = 0, out = 0;
int size=0;
int consumed =0;
void *producer(void *param);
void *consumer(void *param);


int insert_item(buffer_item item)
{

    //Semaphore
      printf("Insert fucntion***************\n");
	sem_wait(&empty);
	
	//Acquire mutex 
	pthread_mutex_lock(&mutex);
	buffer[in++] = item;
	size++;// add total producer 
	
	printf("Insert_item %d at position %d \n", item, (in%5 )-1);
	
	in = in % 5;
		for(int a=0; a<BUFFER_SIZE; a++){
		printf("Buffer: [%d] ", buffer[a]); //print buffer 
		
		}
		printf("In = %d out =%d \n",in,out);// print in and out values 
	//Release mutex 
	pthread_mutex_unlock(&mutex);
      sem_post(&full);
	
	return 0;
}

int remove_item(buffer_item *item)
{
	//get Semaphore
      printf("Remove Function: ################\n");
	sem_wait(&full);

	//get lock
	pthread_mutex_lock(&mutex);
	*item = buffer[out];
	buffer[out++] = -1;
	printf("Remove_item %d at position %d \n", *item, (out%5)-1);
	out = out % 5;
	consumed++;
	for(int a=0; a<BUFFER_SIZE; a++){
		printf("REMOVE BUFFER: [%d] ", buffer[a]);// print remove buffer 
		
		}
		printf("In = %d out =%d \n",in,out);// print in and out
	//Release mutex 
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);

	return 0;
}


int main(int argc, char *argv[])
{
	int sleepMain, producerT, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "So enter First number Main sleep second number Number of producers and third number as number of consumers\n");
		return -1;
	}

	sleepMain = atoi(argv[1]);
	producerT = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	//Initialize the the locks
	printf("pthread_mutex_init: %d\n",pthread_mutex_init(&mutex, NULL));
	sem_init(&empty, 0, BUFFER_SIZE); //Initially, empty value should be larger than 0 
	sem_init(&full, 0, 0);
      printf("-------Main thread beginning------------\n");
	srand(time(0));

	//Create the producer and consumer threads
	for(i = 0; i < producerT; i++)
	{
		pthread_t threadID;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&threadID,&attr, producer, NULL);
		printf("Creating producer thread with id = %lu\n", threadID);
		
	}

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_t threadID;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&threadID, &attr, consumer, NULL);
		printf("Creating consumer thread with id = %lu\n", threadID);
	}
	printf("Main thread sleeping for %d seconds \n\n", sleepMain);
	//specified time Sleep for user 
	sleep(sleepMain);
	printf("Total Product produced: %d Total Consumed: %d \n",size,consumed);
	printf("Main thread exiting------------ \n");
	return 0;
}

void *producer(void *param)
{
	buffer_item item;
	int b;

	while(TRUE)
	{
	
		
		b = (rand() % 3) + 1;
		
		sleep(b);// make the producer sleep 
		
		item = (rand() % 100) + 1; // asign random items 1 therw 100
		
		
		
		printf("Producer thread %lu : Sleeps for: %d \n",pthread_self(), b);
		if(insert_item(item))
			fprintf(stderr, "Error");

		printf("Producer thread %lu Made value-------- %d \n\n\n", pthread_self(),item);
		
	}

}

void *consumer(void *param)
{
	buffer_item item;// getting item
	int a;

	while(TRUE)
	{
		a = (rand() % 3) + 1;
		
		 sleep(a);
		printf("Consumer thread ID: %lu --> Sleeps for: %d \n",pthread_self(), a);
		
		
		
		if(remove_item(&item)) //point to item send item to remove function
			fprintf(stderr, "Error Consuming");
		else
			printf("Consumer thread %lu Eat value %d \n\n\n", pthread_self(),item);
	}
}
