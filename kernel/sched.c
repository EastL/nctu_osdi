#include <kernel/task.h>
#include <inc/x86.h>

#define ctx_switch(ts) \
  do { env_pop_tf(&((ts)->tf)); } while(0)

/* TODO: Lab5
* Implement a simple round-robin scheduler (Start with the next one)
*
* 1. You have to remember the task you picked last time.
*
* 2. If the next task is in TASK_RUNNABLE state, choose
*    it.
*
* 3. After your choice set cur_task to the picked task
*    and set its state, remind_ticks, and change page
*    directory to its pgdir.
*
* 4. CONTEXT SWITCH, leverage the macro ctx_switch(ts)
*    Please make sure you understand the mechanism.
*/
void sched_yield(void)
{
	extern Task tasks[];
	extern Task *cur_task;
	Task *t;

	int task_index = cur_task ? (cur_task->task_id) : 0;
	int index;

	for (index = 0; index < NR_TASKS; index++)
	{
		if (tasks[(task_index+1) % NR_TASKS].state == TASK_RUNNABLE)
		{
			t = &tasks[(task_index+1) % NR_TASKS];
			t->state = TASK_RUNNING;
			cur_task->state = TASK_RUNNABLE;
			cur_task = t;
//			printk("%d\n", t->task_id);
			lcr3(PADDR(t->pgdir));
			ctx_switch(t);
		}
	}
	
	cur_task->remind_ticks = TIME_QUANT;
	
}
