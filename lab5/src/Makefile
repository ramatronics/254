all: thread

thread:
	gcc -o produce producer_consumer_thread.c -lpthread -lm

process:
	gcc -o produce_p producer.c -lrt
	gcc -o consume_p consumer.c -lrt -lm
	gcc -o produce producer_consumer_process.c -lrt
