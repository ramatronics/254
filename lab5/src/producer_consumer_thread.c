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

int spawn (char* program, char** arg_list, mqd_t qdes, int pid){
	arg_list[0] = program;


	/* need to cast the pid to a string to be passed into the process */
	char pid_string[20];
	sprintf(pid_string, "%d", pid);
	arg_list[4] = pid_string;


	pid_t child_pid;
	child_pid = fork();

	if (child_pid != 0){
		return child_pid;
	} else {
		execvp(program, arg_list);
		printf("%s\n", "an error has occurred in exec");
		abort();
	}
}

int main(int argc, char *argv []){
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

	struct mq_attr attr;
	mqd_t qdes;
	mode_t mode = S_IRUSR | S_IWUSR;

	attr.mq_maxmsg  = buffer_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;

	char *qname = "/mailbox_lab4_extended";

	//open the queue
	qdes = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if(qdes == -1){
		perror("mq_open() failed");
		exit(1);
	}

	starting_time = time_in_seconds();

	int i;
	for(i = 0; i < producer_count; i++){
		spawn("/producer_p",argv, qdes, i);
	}

	for(i = 0; i < consumer_count; i++){
		spawn("/consumer_p",argv, qdes, i);
	}

	finished_init_time = time_in_seconds();


	//waiting for all the child process to finish before continuing
	int pid;
	while (pid = waitpid(-1, NULL, 0)) {
	   if (errno == ECHILD) {
	      break;
	   }
	}

	ending_time = time_in_seconds();

	execution_time = starting_time - ending_time;

	printf("System Excution time: %f seconds\n", execution_time);


	/* closing and unlinking the message queue */

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