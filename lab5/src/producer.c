#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>


int main(int argc, char* argv[]){
	int pid;
	int total_message_number;
	int buffer_size;
	int producer_count;
	int consumer_count;

	
	total_message_number = atoi(argv[1]);
	pid = atoi(argv[2]);
	producer_count = atoi(argv[3]);
	consumer_count = atoi(argv[4]);

	mqd_t qdes;
	char *qname = "/mailbox_lab4_extended";

	//open the queue
	printf("pid: %d, opening the queue\n", pid );
	qdes = mq_open(qname,  O_RDWR);
	if(qdes == -1){
		perror("mq_opne()");
		exit(1);
	}

	/* produce elements */

	int i;
	for(i = pid; i < total_message_number; i += producer_count){
		if(mq_send(qdes, (char*)&i, sizeof(int), 0) == -1){
			perror("mq_send() failed");
		}		
	}

	if (mq_close(qdes) == -1){
		perror("mq_close() failed");
		exit(2);
	}

	if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
	}

	return 0;

}






// int spawn (char* program, char** arg_list, mqd_t qdes) {

// 	arg_list[0] = program;

// 	pid_t child_pid;
// 	child_pid = fork();

// 	if (child_pid != 0) {
// 		return child_pid;
// 	} else {
// 		// ./consume is the compiled file of consumer.c
// 		execvp ("./consume", arg_list);

// 		fprintf(stderr, "an error occurred in exec\n" );
// 		abort ();
// 	}
// }

// double time_in_seconds(){
// 	struct timeval tv;
// 	double t1;

// 	gettimeofday(&tv, NULL);
// 	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

// 	return t1;
// }


// int main(int argc, char *argv[]) {

// 	/* ensure valid input */

// 	if (argc < 3){
// 		printf("Invalid Inputs\n");
// 		exit(0);
// 	}


// 	/* process the inputs */

// 	int number_of_messages;
// 	int queue_size;

// 	number_of_messages = atoi(argv[1]);
// 	queue_size = atoi(argv[2]);

// 	if (number_of_messages < 0 || queue_size < 0) {
// 		printf("Invalid inputs");
// 		exit(1);
// 	}

// 	/* setting up and trying to open the message queue for the producer */

// 	struct mq_attr attr;                // message queue attributes
// 	mqd_t qdes;                         // message queue struct
// 	mode_t mode = S_IRUSR | S_IWUSR;    // some kind of mode?

// 	attr.mq_maxmsg  = queue_size;
// 	attr.mq_msgsize = sizeof(int);
// 	attr.mq_flags   = 0;		        // a blocking queue 

// 	char *qname = "/mailbox_lab4";

// 	//open the queue
// 	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
// 	if (qdes == -1 ) {
// 		perror("mq_open() failed");
// 		exit(1);
// 	}

// 	/* forking a child proccess to be consumer */

// 	double starting_time = time_in_seconds();
// 	pid_t child_pid = spawn("consumer", argv, qdes);
// 	double time_at_first_int = time_in_seconds();

// 	/* sending random numbers from the producer into the message queue */

// 	int i = 0;
// 	int *random_number;
// 	int message_priority = 0;
// 	srand(time(0));

// 	for(i = 0; i < number_of_messages; i++ ){
// 		random_number  = rand() % 100;

// 		//send message to the queue
// 		if(mq_send(qdes, (char*)&random_number, sizeof(int), message_priority) == -1){
// 			perror("mq_send() failed");
// 		}
// 	}

// 	/* ensure that the consumer has consumed all the ints */
	
// 	int return_status;
// 	waitpid(child_pid, &return_status, 0);

// 	if(return_status == 0) {
// 		double ending_time = time_in_seconds();
// 		double time_to_initialize = time_at_first_int - starting_time ;
// 		double time_to_consume = ending_time - time_at_first_int;


// 		printf("Time to initialize system: %f\n", time_to_initialize );
// 		printf("Time to transmit data: %f\n", time_to_consume);

// 	} else {
// 		printf("There has been an error in the child process");
// 		exit(1);
// 	}



// 	/* closing and unlinking the message queue */

// 	if (mq_close(qdes) == -1){
// 		perror("mq_close() failed");
// 		exit(2);
// 	}

// 	if (mq_unlink(qname) != 0) {
// 		perror("mq_unlink() failed");
// 		exit(3);
// 	}

// 	return 0;



// }
