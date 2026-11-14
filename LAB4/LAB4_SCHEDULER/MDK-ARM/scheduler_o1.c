#include <string.h>
#include "scheduler.h"
#include "sch_port.h"

typedef struct{
	void (*pTask)(void);
	volatile uint32_t Delay;
	uint32_t Period;
	volatile uint8_t RunMe;									// flag, add the head of delta list
	uint32_t TaskID;
	
}sTask;

static sTask SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t SCH_count = 0;						// the number of task in the list
static uint32_t newTaskID = 0;					// ID have been added nearest

static uint32_t Get_New_Task_ID(void){
	if(++newTaskID == NO_TASK_ID) newTaskID++;				// Create specific ID for new Task != 0 start 1
	return newTaskID;
}

void SCH_Init(void){
	memset(SCH_tasks_G, 0, sizeof(SCH_tasks_G));			// clear all the array = 0
	SCH_count = 0;
	newTaskID = 0;
}

void SCH_Update(void){
	if(SCH_count == 0) return;
	if(SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;				// 1 decrease =  10ms
	if(SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;			// flag to run this task
}

static uint32_t Insert(void (*pF)(void), uint32_t delay, uint32_t period, uint32_t id){
	uint32_t m = SCH_Lock();
	if(SCH_count >= SCH_MAX_TASKS){
		SCH_Unlock(m);														// return the previous state before exit
		return NO_TASK_ID;
	}
	
	uint8_t idx = 0;
	uint32_t sum = 0;
	while(idx < SCH_count && sum + SCH_tasks_G[idx].Delay <= delay){				// find the place to insert new task into delta list
		sum += SCH_tasks_G[idx].Delay;
		idx++;
	}
	
	for(uint8_t i = SCH_count; i > idx; i--){									// insert at tail idx = SCH_count
		SCH_tasks_G[i] =  SCH_tasks_G[i - 1];										// shift all array to right inorder to insert new task
	}
	
	sTask *t = &SCH_tasks_G[idx];															// t tro den slot vua tao ra de chen task
	t->pTask = pF;
	t->Delay = delay - sum;																		// sum = 0 at head
	t->Period = period;
	t->TaskID = id;
	t->RunMe = 0;
	if(idx == 0 && t->Delay == 0) t->RunMe = 1;										// new task at head && time out
	
	if(idx < SCH_count)
		SCH_tasks_G[idx + 1].Delay -= t->Delay;									// update delta list when insert new task at head
	
	SCH_count++;
	SCH_Unlock(m);
	return id;
}

// ADD TASK
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD){
	uint32_t m = SCH_Lock();
	uint32_t id = Get_New_Task_ID();											// allocate new id
	SCH_Unlock(m);
	return Insert(pFunction, DELAY, PERIOD, id);					// return id where insert
}

//DISPATCHER
void SCH_Dispatch_Tasks(void){
	while(SCH_count > 0 && SCH_tasks_G[0].RunMe){							// run all task at the same time when time out
		 sTask t = SCH_tasks_G[0];
		t.pTask();
		if(SCH_Delete_Task(t.TaskID) && t.Period > 0)						// delete and insert back if period > 0
			Insert(t.pTask, t.Period, t.Period, t.TaskID);
		
	}
}

// DELETE TASK
uint8_t SCH_Delete_Task(uint32_t taskID){
	if(taskID == NO_TASK_ID) return 0;
	uint8_t found = 0;
	uint32_t m = SCH_Lock();
	
	for(uint8_t i = 0; i < SCH_count; i++){
		if(SCH_tasks_G[i].TaskID != taskID) continue;
		
		if(i + 1 < SCH_count){
			SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;				// delete task although this have not timed out
			SCH_tasks_G[i + 1].RunMe = 0;
			if(i == 0 && SCH_tasks_G[i + 1].Delay == 0)
				SCH_tasks_G[i + 1].RunMe = 1;
		}
		
		for(uint8_t j = i; j + 1 < SCH_count; j++)
			SCH_tasks_G[j] = SCH_tasks_G[j + 1];									// shift left all task after the place was deleted
		
		SCH_count--;
		memset(&SCH_tasks_G[SCH_count], 0, sizeof(sTask));			// delete the final slot 
		found = 1;
		break;
	}
	
	SCH_Unlock(m);
	return found;
}
