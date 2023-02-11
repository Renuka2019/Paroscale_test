#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

void add_data(int data);
int remove_data();
void *producer(void *arg);
void *consumer(void *arg);

typedef struct node {
	int data;
	struct node *next;
} Node;

Node *head = NULL, *tail = NULL;
int count = 0;
sem_t full, empty;
pthread_mutex_t lock;

void add_data(int data) 
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = NULL;
	/* Incorrect Condition : We have to check if list is empty add new node 
       as a head in the list. So, condition should be -
       if(head == NULL)
       {  
       		head = tail = new_node;
       }
    */

	if(head == NULL) 
	{ 
		head = tail = new_node;
	} 
	else 
	{
		tail->next = new_node;
		tail = new_node;
	}
	count++;
}

int remove_data() 
{
	Node *temp = head;
	int data = temp->data;
	head = head->next;
	count--;
	/*
        Here we are decrementing the count but deallocation of memory is
        not done. we have to free the memory of that node which is consumed 
        by consumer. For that after count-- we have to do :

        free(temp);
        temp = NULL; 
     */

	free(temp);
	temp = NULL;
	return data;
}

void *producer(void *arg) 
{
	int i, data;
	for(i=0; i<100; i++) 
	{
		data = rand() % 100;
		sem_wait(&empty);
		pthread_mutex_lock(&lock);
		add_data(data);
		printf("Produced: %d\n", data);
		pthread_mutex_unlock(&lock);
		sem_post(&full);
	}
}

void *consumer(void *arg) 
{
	int i, data;
	for(i=0; i<100; i++) 
	{
		sem_wait(&full);
		pthread_mutex_lock(&lock);
		data = remove_data();
		printf("Consumed: %d\n", data);
		pthread_mutex_unlock(&lock);
		sem_post(&empty); 
	}
}

int main() 
{
	pthread_t producer_thread, consumer_thread;
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_mutex_init(&lock, NULL);
	pthread_create(&producer_thread, NULL, producer, NULL);
	pthread_create(&consumer_thread, NULL, consumer, NULL);
	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);
	return 0;
}