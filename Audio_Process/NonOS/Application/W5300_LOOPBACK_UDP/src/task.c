/*************************************************************
	filename:	task.c
*************************************************************/
#include "dsp28x_project.h"
#include "task.h" 

/*** Global variables ***/
TASK tasks[MAX_TASKS];

void delTask(Uint16 taskIndex)
{                     
	tasks[taskIndex].ptrTask=0;
	tasks[taskIndex].delay=0;
	tasks[taskIndex].period=0;
	tasks[taskIndex].runMe=0;
	tasks[taskIndex].co_op=0;
}//end of delTask()    
                                
                                
void addTask(void(*ptr)(),Uint16 delay,Uint16 period,Uint16 co_op)
{                  
	Uint16 index=0;
	
	while((tasks[index].ptrTask!=0)&(index<MAX_TASKS)) index++;
	
	tasks[index].ptrTask=ptr;
	tasks[index].delay=delay;
	tasks[index].period=period;
	tasks[index].co_op=co_op;
	tasks[index].runMe=0;
}//end of addTask() 

void dispatchTask(void)
{                      
	Uint16 index;	
	
	for(index=0;index<MAX_TASKS;index++){
		if((tasks[index].co_op)&(tasks[index].runMe>0)){
			(*tasks[index].ptrTask)();
			tasks[index].runMe--;
			
			if(tasks[index].period==0)
				tasks[index].ptrTask=0;
		}
	}
}//end of dispatchTask()     

interrupt void taskUpdate(void)
{
	Uint16 index;	

	for(index=0;index<MAX_TASKS;index++){
		if(tasks[index].ptrTask){
			if(tasks[index].delay==0){
				if(tasks[index].co_op) 
					tasks[index].runMe++;
				else{
					(*tasks[index].ptrTask)();             	
					tasks[index].runMe--;
					
					if(tasks[index].period==0) tasks[index].ptrTask=0;
				}
				
				if(tasks[index].period) tasks[index].delay=tasks[index].period;										
			}
			else
				tasks[index].delay--;
		}
	}  	
	
	// Acknowledge this interrupt to receive more interrupts from group 1
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}//end of taskUpdate()

void taskInit(void)
{
	Uint16 i;	
	
	for(i=0;i<MAX_TASKS;i++)	
		delTask(i);			
}/* end of taskInit() */

/*** END ****************************************************/
