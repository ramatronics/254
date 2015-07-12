#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

struct queue {
	struct queue* next;
	int value;
};

struct queue* message_queue;
pthread_mutex_t message_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t message_count; //makes sure that there is a message in the queue before the consumer tries to consume
sem_t consumed_messages_count; //allow the consumer thread to exit once all the messages have been consumed
sem_t messages_in_queue; //makes sure that there is enough space in the queue to handle one another enqueue 

int producer_count;
int total_number_of_messages;
int message_queue_size;

// a one time init of the job queue

void init_message_queue(){
	message_queue = NULL;
	sem_init(&message_count, 0,0); 
	sem_init(&consumed_messages_count,0,total_number_of_messages);
	sem_init(&messages_in_queue, 0, message_queue_size);
}


void enqueue(int data){
	if(message_queue == NULL){
		message_queue = malloc(sizeof(struct queue));
		message_queue->value = data;
		message_queue->next = NULL;
	} else {
		struct queue* new_message = malloc(sizeof(struct queue));
		new_message->next = message_queue;
		new_message->value = data;
	    message_queue = new_message;
	}
}

void dequeue(int c_id){
	struct queue* next_message;

	next_message = message_queue;
	message_queue= message_queue->next;

	int value = next_message->value;
	int sqrt_val = sqrt(value);

	printf("%i %i %i\n", c_id, value, sqrt_val);

	// if (value == (sqrt_val * sqrt_val)){
	// 	printf("%i %i %i\n", c_id, value, sqrt_val);
	// }

}


//processs thread untill empty

void* dequeue_message(void* arg){
	int c_id = *((int *)arg);	
	while(1){
		
		


		sem_wait(&message_count);

		// lock the message queue from enqueuing or dequeing
		pthread_mutex_lock (&message_queue_mutex);
		dequeue(c_id);
		pthread_mutex_unlock(&message_queue_mutex);

		sem_wait(&messages_in_queue);

		
	}
	return NULL;
}

//enqueue into the message queue

void* enqueue_message (void* arg){
	int p_id = *((int *)arg);
	int i;
	for(i = p_id; i < total_number_of_messages; i += producer_count){
	    pthread_mutex_lock (&message_queue_mutex);

	    //adding a new job to the head of the linked list
	    
	    sem_wait(&messages_in_queue);
	    
	    enqueue(i);

	    // let other threads know that there are jobs in the queue 
	    // one of the threads that was blocked, now has the ability to consume the job;
	    sem_post(&message_count);
	    sem_post(&messages_in_queue);



	    pthread_mutex_unlock(&message_queue_mutex);

	}
	return NULL;
    
}

int main(int argc, char *argv[0]){
	if (argc < 5){
		printf("Invalid number of inputs\n");
		exit(0);
	}

	int consumer_count;

	total_number_of_messages = atoi(argv[1]);
	message_queue_size = atoi(argv[2]);
	producer_count = atoi(argv[3]);
	consumer_count = atoi(argv[4]);

	if (total_number_of_messages < 0 || message_queue_size < 0 || producer_count < 0 || consumer_count < 0){
		printf("all args must be positive integers");
		exit(1);
	}

	init_message_queue();

	//create an array of producer and consumer thread ids

	pthread_t producer_thread_id[producer_count];
	pthread_t consumer_thread_id[consumer_count];

	int c_ids[consumer_count];
	int p_ids[producer_count];

	//create the producer and consumer threads
	int i;
	for(i = 0; i < producer_count; i++){
		p_ids[i] = i;
		pthread_create(&(producer_thread_id[i]), NULL, &enqueue_message, (void*)&(p_ids[i]));
	}

	for(i=0; i < consumer_count; i++){
		c_ids[i] = i;
		pthread_create(&(consumer_thread_id[i]), NULL, &dequeue_message, (void*)&(c_ids[i]));
	}


	// joing the producer and consumer threads to avoid exiting the main thread
	// before the other threads have been completed

	for(i=0; i < producer_count; i++){
		pthread_join(producer_thread_id[i], NULL);
	}

	for(i=0; i < consumer_count; i++){
		pthread_join(consumer_thread_id[i], NULL);
	}

	sem_destroy(&message_count);
	sem_destroy(&consumed_messages_count);
	sem_destroy(&messages_in_queue);

	return 0;
}
