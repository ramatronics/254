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
#include <math.h>



int main(int argc, char *argv[]) {

	/* processing inputs */
	int number_of_messages = atoi(argv[1]);
	int cid = atoi(argv[2]);

	/* opening the message queue and the consumption queue */
	mqd_t qdes;
	char *qname = "/mailbox_lab4_extended";

	mqd_t consumption_queue;
	char *cmqname = "/consumption_queue_mailbox";

	qdes  = mq_open(qname, O_RDONLY);
	if (qdes == -1 ) {
		perror("mq_open()");
		exit(1);
	}
	
	consumption_queue = mq_open(cmqname, O_RDWR);
	if (consumption_queue == -1){
		perror("mq_open() failed from consumer");
		exit(1);
	}


	/* consume elements */

	int i;
	int count = 0;
	int sqrt_val;
	
	while(1){
		int recieved_message;
		int consumption_count;

		/* checking the number of messages that have been consumed in total.
		   stopping the process if the consumption count == 0
		*/

		mq_receive(consumption_queue, (char *)&consumption_count, sizeof(int), 0);
		if (consumption_count ==  0){
			mq_send(consumption_queue, (char *)&consumption_count, sizeof(int), 0);
			break;
		} else {
			consumption_count --;
			mq_send(consumption_queue, (char *)&consumption_count, sizeof(int), 0);
		}

		/* dequeue the message from the message queue */

		if (mq_receive(qdes, (char *)&recieved_message, sizeof(int), 0) == -1){
			printf("mq_receive() failed\n");
			return 1;
		} 

		/* prints the values if perfect square */
		sqrt_val = sqrt(recieved_message);
		if((sqrt_val * sqrt_val) == recieved_message){
			printf("%i %i %i\n", cid, recieved_message, sqrt_val);
		}

		

	}

	/* closing the message queue on the consumer side */

	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	/* closing the consumption queue on the consumer side */

	if(mq_close(consumption_queue) == -1){
		perror("mq_close() failed");
		exit(2);
	}

	return 0;

}
