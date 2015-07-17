all: thread

thread:
	gcc -o produce producer_consumer.c -lpthread -lm

process:
	gcc -o produce_p producer.c -lrt
	gcc -o consume_p consumer.c -lrt
	gcc -o producer_consumer_thread.c -lrt
