#ifndef	_TASK_H_
#define _TASK_H_

#define MAX_TASKS	7

typedef struct{
	void (*ptrTask)(void);
	Uint16	delay;
	Uint16	period;
	Uint16	runMe;
	Uint16	co_op;
	}TASK;

extern TASK tasks[MAX_TASKS];

extern void delTask(Uint16 taskIndex);                                
extern void addTask(void(*ptr)(),Uint16 delay,Uint16 period,Uint16 co_op);
extern void dispatchTask(void);
extern interrupt void taskUpdate(void); 
extern void taskInit(void);

#endif























