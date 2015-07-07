#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char *argv[]) {

	/* processing inputs */
	int number_of_messages = atoi(argv[1]);
	int queue_size = atoi(argv[2]);


	/* opening the message queue for the consumer */

	mqd_t qdes;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = queue_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;	/* a blocking queue  */

	char *qname = "/mailbox_lab4";

	//open the queue
	qdes  = mq_open(qname, O_RDONLY, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open()");
		exit(1);
	}


	/* consume elements */

	int i;
	int message_priority = 0;
	for(i = 0; i < number_of_messages; i++){
		int recieved_message;

		//receive message from the queue
		if (mq_receive(qdes, (char *)&recieved_message, sizeof(int), message_priority) == -1){
			printf("mq_receive() failed\n");
			return 1;
		} else {
			printf("%d is consumed\n", recieved_message);
		}

	}

	/* closing the message queue on the consumer side */

	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	return 0;

}