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


//processs thread untill empty

void* dequeue_message(){
	int c_id = (int) (void*) (pthread_self());
	while(1){
		

		if(sem_trywait(&consumed_messages_count)){
			break;
		}

		sem_wait(&message_count);

		// lock the message queue from enqueuing or dequeing
		pthread_mutex_lock (&message_queue_mutex);
		dequeue(c_id);
		pthread_mutex_unlock(&message_queue_mutex);

		sem_wait(&messages_in_queue);

		
	}
}

//enqueue into the message queue

void* enqueue_message (){
	int p_id = (int) (void*) (pthread_self());

	int i;
	for(i = p_id; p_id < message_count; i+= producer_count){
		struct queue* new_message;

	    

	    pthread_mutex_lock (&message_queue_mutex);

	    //adding a new job to the head of the linked list
	    sem_wait(&messages_in_queue);
	    
	    enqueue(p_id);

	    // let other threads know that there are jobs in the queue 
	    // one of the threads that was blocked, now has the ability to consume the job;
	    sem_post(&message_count);
	    sem_post(&messages_in_queue);



	    pthread_mutex_unlock(&message_queue_mutex);

	};
    
}

int main(int argc, char *argv[0]){
	if (argc < 4){
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

	//create the producer and consumer threads
	int i;
	for(i = 0; i < producer_count; i++){
		pthread_create(&(producer_thread_id[i]), NULL, &enqueue_message, NULL);
	}

	for(i=0; i < consumer_count; i++){
		pthread_create(&(consumer_thread_id[i]), NULL, &dequeue_message, NULL);
	}


	// joing the producer and consumer threads to avoid exiting the main thread
	// before the other threads have been completed

	for(i=0; i < producer_count; i++){
		pthread_join(producer_thread_id[i], NULL);
	}

	for(i=0; i < consumer_count; i++){
		pthread_join(consumer_thread_id[i], NULL);
	}

	return 0;
}

void enqueue(int data){
	if(message_queue == NULL){
		new_message = malloc(sizof(queue));
		new_message->value = data;
		new_message->next = NULL;
	} else {
		struct queue* new_message = malloc(sizof(queue));
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

	if (value == (sqrt_val * sqrt_val)){
		printf("%i %i %i\n", c_id, value, sqrt_val);
	}
}