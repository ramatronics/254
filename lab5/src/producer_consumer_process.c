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


double time_in_seconds(){
	struct timeval tv;
	double t1;

	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

	return t1;
}

/* spawn a child process 
   Both the producers and consumers are each their own process
*/

int spawn (char* program, char** arg_list, int pid){
	arg_list[0] = program;


	/* need to cast the pid to a string to be passed into the process 
	   writing to arg_list[2] because the buffer count is not needed for the consumers or producers
	*/
	char pid_string[10];
	sprintf(pid_string, "%d", pid);
	arg_list[2] = pid_string;


	pid_t child_pid;
	child_pid = fork();

	/* error check for child process */

	if (child_pid != 0){
		return child_pid;
	} else {
		execvp(program, arg_list);
		printf("%s\n", "an error has occurred in exec");
		abort();
	}
}

int main(int argc, char *argv []){

	/* process and ensure valid inputs */
	if (argc < 5){
		printf("%s\n", "Invalid number of inputs");
		exit(0);
	}

	int consumer_count;
	int producer_count;
	int buffer_size;
	int total_message_number;

	total_message_number = atoi(argv[1]);
	buffer_size = atoi(argv[2]);
	producer_count = atoi(argv[3]);
	consumer_count = atoi(argv[4]);

	if (total_message_number < 0 || buffer_size < 0 || producer_count < 0 || consumer_count < 0){
		printf("%s\n", "All inputs must be positive integers" );
		exit(0);
	}

	double starting_time;
	double finished_init_time;
	double ending_time;
	double execution_time;

	/* create and open the message queue */

	struct mq_attr attr;
	mqd_t qdes;
	mode_t mode = S_IRUSR | S_IWUSR;

	attr.mq_maxmsg  = buffer_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;

	char *qname = "/mailbox_lab4_extended";

	qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if(qdes == -1){
		perror("mq_open() failed");
		exit(1);
	}

	/* create and open a consumption queue
	   consumption queue is a mq of size 1 which keeps track of the number of messages
	   that have been consumed so far
	*/
	struct mq_attr c_attr; //consumption queue attr
	mqd_t consumption_queue;
	mode_t mode_c = S_IRUSR | S_IWUSR;

	c_attr.mq_maxmsg = 1;
	c_attr.mq_msgsize = sizeof(int);
	c_attr.mq_flags = 0;

	char * consumption_queue_name = "/consumption_queue_mailbox";
	
	consumption_queue = mq_open(consumption_queue_name, O_RDWR | O_CREAT, mode_c, &c_attr);
	if (consumption_queue == -1){
		perror("mq_open() for consumption failed");
		exit(1);
	}


	/* we start by writing the value of `total_message_number` into the consumption_queue */
	if(mq_send(consumption_queue, (char *)&total_message_number, sizeof(int), 0) == -1){
		perror("mq_send() for consumption failed");
	}

	starting_time = time_in_seconds();

	/* loop through and create the producers and consumers processes */

	int i;

	for(i = 0; i < consumer_count; i++){
		spawn("./consume_p",argv, i);
	}

	for(i = 0; i < producer_count; i++){
		spawn("./produce_p",argv, i);
	}

	
	finished_init_time = time_in_seconds();


	/* waiting for all the child processes to finsih before continuing 
	   reference: stack overflow
        */
	int pid;
	while (pid = waitpid(-1, NULL, 0)) {
	   if (errno == ECHILD) {
	      break;
	   }
	}

	ending_time = time_in_seconds();

	/* assumtion: execution time includes the time for initialization */

	execution_time = ending_time - starting_time;

	printf("System Excution time: %f seconds\n", execution_time);


	/* closing and unlinking the message queue and consumption queue */

	if (mq_close(qdes) == -1){
		perror("mq_close() failed");
		exit(2);
	}

	if(mq_close(consumption_queue) == -1){
		perror("mq_close() for cmq failed from main");
		exit(2);
	}

	if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
	}

	if(mq_unlink(consumption_queue_name) == -1){
		perror("mq_unlink() failed for cmq from main");
		exit(3);
	}

	return 0;


}
