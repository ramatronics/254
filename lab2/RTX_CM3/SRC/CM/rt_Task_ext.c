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