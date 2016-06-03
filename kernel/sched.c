#include <kernel/task.h>
#include <kernel/cpu.h>
#include <inc/x86.h>

//<<<<<<< HEAD
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

//
// TODO: Lab6
// Modify your Round-robin scheduler to fit the multi-core
// You should:
//
// 1. Design your Runqueue structure first (in kernel/task.c)
//
// 2. modify sys_fork() in kernel/task.c ( we dispatch a task
//    to cpu runqueue only when we call fork system call )
//
// 3. modify sys_kill() in kernel/task.c ( we remove a task
//    from cpu runqueue when we call kill_self system call
//
// 4. modify your scheduler so that each cpu will do scheduling
//    with its runqueue
//    
//    (cpu can only schedule tasks which in its runqueue!!) 
//    (do not schedule idle task if there are still another process can run)	
//
/*
=======
static int index = 0;
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
void sched_yield(void)
{
	/* Lab4: Implement a simple round-robin scheduling there */
	extern Task tasks[];
//<<<<<<< HEAD
	//extern Task *cur_task;

	int task_i = thiscpu->cpu_task ? (thiscpu->cpu_rq.current_index) : 0;
	int i, index, task_index;
	int size;
	size = thiscpu->cpu_rq.total;
	//printk("%d ", cpus[1].cpu_rq.total);
	//printk("%d\n", size);

	for (i = 0; i < size; i++)
	{
		//index = thiscpu->cpu_rq.runq[i];
		task_i++;
		task_index = thiscpu->cpu_rq.runq[task_i % size];
		if (tasks[task_index].state == TASK_RUNNABLE)
		{
			thiscpu->cpu_task = &tasks[task_index];
			thiscpu->cpu_task->state = TASK_RUNNING;
			thiscpu->cpu_rq.current_index = task_i;
			lcr3(PADDR(thiscpu->cpu_task->pgdir));
			ctx_switch(thiscpu->cpu_task);
		}
	}
	
	//if (thiscpu->cpu_task->task_id == 0)
	//	thiscpu->cpu_task->remind_ticks = TIME_QUANT;
	
/*
=======
	extern Task *cur_task;
	int i;
	int next_i = 0;

	i = (index +1)%NR_TASKS;
	while (1)
	{
		if ((tasks[i].state == TASK_RUNNABLE))
		{
			next_i = i;
			break;
		}
		if (++i >= NR_TASKS) i = 0;

		if (index == i)
		{
			next_i = -1;
			break;
		}
		

	}

	if (next_i == -1 ) //only one task can run
		next_i = index;

	if (next_i >= 0 && next_i < NR_TASKS)
	{
		cur_task = &(tasks[next_i]);
		cur_task->remind_ticks = TIME_QUANT;
		cur_task->state = TASK_RUNNING;
		index = next_i;
    lcr3(PADDR(cur_task->pgdir));
		env_pop_tf(&(cur_task->tf));
	}

>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
}
