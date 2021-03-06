/* Reference: http://www.osdever.net/bkerndev/Docs/pit.htm */
#include <kernel/trap.h>
#include <kernel/picirq.h>
#include <kernel/task.h>
#include <kernel/cpu.h>
#include <inc/mmu.h>
#include <inc/x86.h>

#define TIME_HZ 100

static unsigned long jiffies = 0;

void set_timer(int hz)
{
  int divisor = 1193180 / hz;       /* Calculate our divisor */
  outb(0x43, 0x36);             /* Set our command byte 0x36 */
  outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
  outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* It is timer interrupt handler */
//
// TODO: Lab6
// Modify your timer_handler to support Multi processor
// Don't forget to acknowledge the interrupt using lapic_eoi()
//
void timer_handler(struct Trapframe *tf)
{
//<<<<<<< HEAD
	extern void sched_yield();
	int i;

	jiffies++;

	lapic_eoi();
	extern Task tasks[];

	//extern Task *cur_task;

	if (thiscpu->cpu_task != NULL)
	{
		/* TODO: Lab 5
		* 1. Maintain the status of slept tasks
		* 
		* 2. Change the state of the task if needed
		*
		* 3. Maintain the time quantum of the current task
		*
		* 4. sched_yield() if the time is up for current task
		*
		*/

		int index;

		int size = thiscpu->cpu_rq.total;
		for (i = 0; i < size; i++)
		{
			index = thiscpu->cpu_rq.runq[i];
			if (tasks[index].state == TASK_SLEEP)
			{
				tasks[index].remind_ticks--;
				if (tasks[index].remind_ticks == 0)
				{
					tasks[index].state = TASK_RUNNABLE;
					tasks[index].remind_ticks = TIME_QUANT;
				}
			}
		}	

		if (--thiscpu->cpu_task->remind_ticks == 0)
		{
			thiscpu->cpu_task->state = TASK_RUNNABLE;
			thiscpu->cpu_task->remind_ticks = TIME_QUANT;
			sched_yield();
		}
	}
/*
=======
  extern void sched_yield();
  int i;

  jiffies++;

  // Lab4: Check is need wakeup sleep task 
  extern Task tasks[];

  extern Task *cur_task;

  if (cur_task != NULL)
  {
    // Lab4: Check is need wakeup sleep task 
    for (i = 0; i < NR_TASKS; i++)
    {
      if (tasks[i].state == TASK_SLEEP)
      {
        tasks[i].remind_ticks--;
        if (tasks[i].remind_ticks <= 0)
          tasks[i].state = TASK_RUNNABLE;
      }
    }
    // Lab4: Check cur_task->remind_ticks, if remind_ticks <= 0 then yield the task
    cur_task->remind_ticks--;
    if (cur_task->remind_ticks <= 0)
    {
      cur_task->state = TASK_RUNNABLE;
      sched_yield();
    }
  }
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
}

unsigned long sys_get_ticks()
{
  return jiffies;
}
void timer_init()
{
  set_timer(TIME_HZ);

  /* Enable interrupt */
  irq_setmask_8259A(irq_mask_8259A & ~(1<<IRQ_TIMER));

  /* Register trap handler */
  extern void TIM_ISR();
  register_handler( IRQ_OFFSET + IRQ_TIMER, &timer_handler, &TIM_ISR, 0, 0);
}

