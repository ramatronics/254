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

	/* Open the message queue from the producer side */

	mqd_t qdes;
	char *qname = "/mailbox_lab4_extended";

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

	/* close the message queue from the producer side */

	if (mq_close(qdes) == -1){
		perror("mq_close() failed");
		exit(2);
	}

	return 0;

}

