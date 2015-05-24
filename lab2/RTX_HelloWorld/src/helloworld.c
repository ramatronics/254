/**
* @file: helloworld.c
* @brief: Two simple tasks running pseduo-parallelly
*/
#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "uart_polling.h"
#include "../../RTX_CM3/INC/RTL_ext.h"

__task void task1()
{
	
	int i = 0;
	printf("Task1: Started.\n");
	
	for(;; i++)
	{
		printf("Task1: %d\n", i);
		os_dly_wait(1000);
	}
}
	
__task void task2()
{
	int i=0;
	printf("Task2: Started.\n");
	
	while(i < 5)
	{
		printf("Task2: HelloWorld!\n");
		os_dly_wait(500);
		i++;
	}
	
	printf("Task2: Terminating.\n");
	os_tsk_delete_self();
}

__task void task3()
{
	int num;
	
	printf("Task3: Started.\n");
	while (1) {
		num = os_tsk_count_get();
		printf("Task 3: There are currently %d. tasks running\n", num);
		os_dly_wait(700);
	}
}

__task void task4()
{
	printf("Task4: Started.\n");
	while(1)
	{
		os_dly_wait(1000);
	}
}

__task void task5()
{
	int num;
	
	printf("Task5: Started.\n");
	while (1) {
		num = os_tsk_count_get();
		printf("Task 5: There are currently %d. tasks running\n", num);
		os_dly_wait(700);
	}
}

__task void init(void)
{
	printf("Main Task: Started.\n");
	
	os_tsk_create(task1, 1); // task1 at priority 1
	os_dly_wait(500);
	
	os_tsk_create(task2, 1); // task2 at priority 1
	os_dly_wait(500);
	
	os_tsk_create(task3, 1); // task3 at priority 1
	os_dly_wait(500);
		
	os_tsk_create(task4, 1); // task4 at priority 1
	os_dly_wait(500);
	
	os_tsk_create(task5, 1); // task5 at priority 1
	os_dly_wait(500);
	
	printf("Main Task: There are currently %d tasks running\n", os_tsk_count_get());
	
	printf("Main Task: Terminating.\n");
	os_tsk_delete_self(); // must delete itself before exiting
}

int main ()
{
	SystemInit();
	uart0_init();
	os_sys_init(init);
}