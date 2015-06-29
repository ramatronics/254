/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

int rt_tsk_count_get (void) {
		//Counter for total number of tasks running
    int totalTasks = 0;	
    int i = 0;
    
		//Pointer for iterating through tasks
    P_TCB currTask;
	
    for(i=0; i < os_maxtaskrun; i++){
				//Get task at index i as P_TCB
        currTask = os_active_TCB[i];
        
				//Check if task is null/exists
        if(currTask != 0){					
						//Increment counter if state isn't inactive
            if(currTask->state != INACTIVE){
                totalTasks++;
            }
        }
    }
    
    return totalTasks;
}

OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {	
	P_TCB t;
	U32 sSize, sAddrEnd, sAddrCurr;
	U32 *sStart, *sEnd;
	
	if((task_id < 1 || task_id > os_maxtaskrun) && task_id != 0xFF)
		return OS_R_NOK;
	
	if((P_TCB)os_active_TCB[task_id - 1] == NULL)
		return OS_R_NOK;
	
	t = task_id == 0xFF ? (P_TCB)&os_idle_TCB : (P_TCB)os_active_TCB[task_id - 1];

	if(p_task_info == NULL || t == NULL)
		return OS_R_NOK;
		
	p_task_info->task_id     = t->task_id;
 	p_task_info->state       = t->state;
	p_task_info->prio        = t->prio;
	p_task_info->ptask       = t->ptask;	

	sSize = t->priv_stack >> 2;
	if(sSize == 0)
		sSize = (U16)os_stackinfo >> 2;
	
	sStart = &t->stack[0];
	sEnd = &t->stack[sSize];
	
	sAddrEnd = (U32)sEnd;
	sAddrCurr = t->state == RUNNING ? rt_get_PSP() : (U32)t->tsk_stack;
	
	p_task_info->stack_usage = (U32)((sAddrEnd - sAddrCurr)*100/(U16)os_stackinfo);
	
	return OS_R_OK;
}