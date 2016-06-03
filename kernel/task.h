#ifndef TASK_H
#define TASK_H

#include <inc/trap.h>
#include <kernel/mem.h>
#define NR_TASKS	30
#define TIME_QUANT	100

typedef enum
{
	TASK_FREE = 0,
	TASK_RUNNABLE,
	TASK_RUNNING,
	TASK_SLEEP,
	TASK_STOP,
	TASK_IDLE,
} TaskState;

// Each task's user space
#define USR_STACK_SIZE	(40960)

typedef struct
{
	int task_id;
	int parent_id;
	int cpu_id;
	struct Trapframe tf; //Saved registers
	int32_t remind_ticks;
	TaskState state;	//Task state
	pde_t *pgdir;  //Per process Page Directory
	
} Task;

//<<<<<<< HEAD
// TODO Lab6
// 
// Design your Runqueue structure for cpu
// your runqueue sould have at least two
// variables:
//
// 1. an index for Round-robin scheduling
//
// 2. a list indicate the tasks in the runqueue
//
typedef struct
{
	int total;
	int current_index;
	int runq[NR_TASKS];

} Runqueue;


void task_init();
void task_init_percpu();
/*
=======
int sys_fork();
void task_init();
void sys_kill(int pid);
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
void env_pop_tf(struct Trapframe *tf);
#endif
