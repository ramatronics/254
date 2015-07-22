#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

/* a queue struct implemented with a circular array 
   it gets allocated as global variable and becomes shared memory 
   that can be accessed by all threads
*/


struct queue {
	int *array;
	int head;
	int tail;
};

struct queue message_queue;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

sem_t message_count;           //makes sure that there is a message in the queue before the consumer tries to consume
sem_t consumed_messages_count; //allow the consumer thread to exit once all the messages have been consumed
sem_t empty_space;             //makes sure that there is enough space in the queue to handle another enqueue 

int producer_count;
int total_number_of_messages;
int message_queue_size;

/* a one time init for the message queue and semaphores */

void init_message_queue(){
	message_queue.array = (int*) malloc (sizeof(int) * message_queue_size);
	message_queue.tail = 0;
	message_queue.head = 0;

	sem_init(&message_count, 0,0); 
	sem_init(&consumed_messages_count,0,total_number_of_messages);
	sem_init(&empty_space, 0, message_queue_size);
}

double time_in_seconds(){
    struct timeval tv;
    double t1;

    gettimeofday(&tv, NULL);
    t1 = tv.tv_sec + tv.tv_usec/1000000.0;

    return t1;
}


void enqueue(int data){
	message_queue.tail++;
	if (message_queue.tail == message_queue_size){
		message_queue.tail = 0;
	}

	message_queue.array[message_queue.tail] = data;
}

void dequeue(int c_id){
	int value, sqrt_val;
	
	message_queue.head++;
	if (message_queue.head == message_queue_size){
		message_queue.head = 0;
	}

	/* printing the value if it is a perfect square */

	value = message_queue.array[message_queue.head];
	sqrt_val = sqrt(value);
	
	if (value == (sqrt_val * sqrt_val)){
		printf("%i %i %i\n", c_id, value, sqrt_val);
	}

}


/* consumer thread function
   keep consuming from the message queue until the `consumed_messsages_count` hits 0
*/

void* dequeue_message(void* arg){
	int c_id = *((int *)arg);	
	while(1){
			
		if(sem_trywait(&consumed_messages_count)){
			break;
		}
		
		//wait for a message to be in the queue
		sem_wait(&message_count);

		// lock the message queue 
		pthread_mutex_lock (&lock);
		dequeue(c_id);
		pthread_mutex_unlock(&lock);

		//signal producers that there is one less message in the message queue
		sem_post(&empty_space);

		
	}
	return NULL;
}

/* producer thread function
   produces using the given algorithm from the manual

   each producer produces a set number of ints
*/

void* enqueue_message (void* arg){
	int p_id = *((int *)arg);
	int i;
	for(i = p_id; i < total_number_of_messages; i += producer_count){

		// make sure that the number of ints in the message queue does not exceed the buffer size
		sem_wait(&empty_space);

	    pthread_mutex_lock(&lock);
	    enqueue(i);
	    pthread_mutex_unlock(&lock);

	    // let consumers know that there is a message in the queue 
	    sem_post(&message_count);  

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

	double starting_time;
	double ending_time;
	double total_time;

	/* create an array of producer and consumer thread ids */

	pthread_t producer_thread_id[producer_count];
	pthread_t consumer_thread_id[consumer_count];

	int c_ids[consumer_count];
	int p_ids[producer_count];

	starting_time = time_in_seconds();

	/* create the producer and consumer threads */

	int i;
	for(i = 0; i < producer_count; i++){
		p_ids[i] = i;
		pthread_create(&(producer_thread_id[i]), NULL, &enqueue_message, (void*)&(p_ids[i]));
	}

	for(i=0; i < consumer_count; i++){
		c_ids[i] = i;
		pthread_create(&(consumer_thread_id[i]), NULL, &dequeue_message, (void*)&(c_ids[i]));
	}


	/* joing the producer and consumer threads to avoid exiting the main thread
	   before the other threads have been completed
	*/

	for(i=0; i < producer_count; i++){
		pthread_join(producer_thread_id[i], NULL);
	}

	for(i=0; i < consumer_count; i++){
		pthread_join(consumer_thread_id[i], NULL);
	}

	ending_time = time_in_seconds();
	total_time = ending_time - starting_time;

	printf("System execution time: %f seconds\n", total_time);

	/* destroying the semaphores */

	sem_destroy(&message_count);
	sem_destroy(&consumed_messages_count);
	sem_destroy(&empty_space);

	return 0;
}
