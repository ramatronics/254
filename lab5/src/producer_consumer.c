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
sem_t message_count;

int producer_count;
int message_count;

// a one time init of the job queue

void init_message_queue(){
	message_queue = NULL;
	sem_init(&message_count, 0,0); 
}


//processs thread untill empty

void* dequeue_message(void* arg){
	while(1){
		struct queue* next_message;

		sem_wait(&message_count);

		// lock the message queue from enqueuing or dequeing
		pthread_mutex_lock (&message_queue_mutex);
		next_message = message_queue;
		message_queue = message_queue->next;
		pthread_mutex_unlock(&message_queue_mutex);

		printf("%d is consumed\n", next_message->value);
	}
}

//enqueue into the message queue

void enqueue_message (int data){
    struct queue* new_message;

    new_message = (struct queue*) malloc (sizeof(struct queue));
    new_message->value = data;

    pthread_mutex_lock (&message_queue_mutex);

    //adding a new job to the head of the linked list
    
    new_message->next = message_queue;
    message_queue = new_message;

    // let other threads know that there are jobs in the queue 
    // one of the threads that was blocked, now has the ability to consume the job;
    sem_post(&message_count);

    pthread_mutex_unlock(&message_queue_mutex);
}

int main(int argc, char *argv[0]){
	if (argc < 4){
		printf("Invalid number of inputs\n");
		exit(0);
	}

	int consumer_count;
	int message_queue_size;

	message_count = atoi(argv[1]);
	message_queue_size = atoi(argv[2]);
	producer_count = atoi(argv[3]);
	consumer_count = atoi(argv[4]);

	if (message_count < 0 || message_queue_size < 0 || producer_count < 0 || consumer_count < 0){
		printf("all args must be positive integers");
		exit(1);
	}

	//create an array of producer and consumer thread ids

	pthread_t producer_thread_id[producer_count];
	pthread_t consumer_thread_id[consumer_count];

	//create the producer threads
	int i;
	for(i = 0; i < producer_count; i++){
		pthread_create(&(producer_thread_id[i]), NULL, &dequeue_message, NULL);
	}

}