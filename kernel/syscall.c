#include <kernel/task.h>
#include <kernel/timer.h>
#include <kernel/mem.h>
//<<<<<<< HEAD
#include <kernel/cpu.h>
#include <kernel/syscall.h>
#include <kernel/trap.h>
/*
=======
#include <inc/syscall.h>
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
#include <inc/stdio.h>
#include <inc/trap.h>

extern void sys_settextcolor(unsigned char forecolor, unsigned char backcolor); // kernel/screen.c
extern void sys_cls(); // kernel/screen.c

void do_puts(char *str, uint32_t len)
{
	uint32_t i;
	for (i = 0; i < len; i++)
	{
		k_putch(str[i]);
	}
}

int32_t do_getc()
{
	int a = k_getc();
	return a;
}

void sys_sleep(uint32_t ticks)
{
	//extern Task *cur_task;
	thiscpu->cpu_task->remind_ticks = ticks;
	thiscpu->cpu_task->state = TASK_SLEEP;
	sched_yield();
}

int sys_getdents(const char *path, const char *buf)
{
	
}

int32_t do_syscall(uint32_t syscallno, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
{
//<<<<<<< HEAD
	int32_t retVal = -1;
	//extern Task *cur_task;

	switch (syscallno)
	{
	case SYS_fork:
	/* TODO: Lab 5
     * You can reference kernel/task.c, kernel/task.h
     */

		retVal = sys_fork();
		break;

	case SYS_getc:
		retVal = do_getc();
		break;

	case SYS_puts:
		do_puts((char*)a1, a2);
		retVal = 0;
		break;

	case SYS_getpid:
		/* TODO: Lab 5
     * Get current task's pid
     */
		retVal = thiscpu->cpu_task->task_id;
		break;

	case SYS_getcid:
		/* Lab6: get current cpu's cid */
		retVal = thiscpu->cpu_id;
		break;

	case SYS_sleep:
		/* TODO: Lab 5
     * Yield this task
     * You can reference kernel/sched.c for yielding the task
     */
		sys_sleep(a1);
		break;

	case SYS_kill:
		/* TODO: Lab 5
     * Kill specific task
     * You can reference kernel/task.c, kernel/task.h
     */
		sys_kill(thiscpu->cpu_task->task_id);
		break;

	case SYS_get_num_free_page:
		/* TODO: Lab 5
     * You can reference kernel/mem.c
     */
		retVal = sys_get_num_free_page();
    	break;

	case SYS_get_num_used_page:
		/* TODO: Lab 5
     * You can reference kernel/mem.c
     */
		retVal = sys_get_num_used_page();
    	break;

	case SYS_get_ticks:
		/* TODO: Lab 5
     * You can reference kernel/timer.c
     */
    	retVal = sys_get_ticks();
    	break;

	case SYS_settextcolor:
		/* TODO: Lab 5
     * You can reference kernel/screen.c
     */
		sys_settextcolor((unsigned char)a1, (unsigned char)a2);
    	break;

	case SYS_cls:
		/* TODO: Lab 5
     * You can reference kernel/screen.c
     */
		sys_cls();
    	break;
        /* TODO: Lab7 file I/O system call */    
        case SYS_open:
		retVal = sys_open((const char*)a1, (int)a2, (int)a3);
	break;
	
        case SYS_read:    
		retVal = sys_read((int)a1, (const void*)a2, (size_t)a3);
	break;
        case SYS_write:
		retVal = sys_write((int)a1, (const void*)a2, (size_t)a3);
	break;
        case SYS_close:   
		retVal = sys_close(a1);
	break;
        case SYS_lseek:
		retVal = sys_lseek(a1, a2, a3);
	break;
        case SYS_unlink:
		retVal = sys_unlink((const char*)a1);
	break;
        case SYS_getdents:
		retVal = sys_getdents(a1, a2);
        break;
/*
=======
    int32_t retVal = -1;
    extern Task *cur_task;
    extern void sched_yield();

    if (syscallno < NSYSCALLS)
    {
        switch (syscallno)
        {
        case SYS_fork:
	        retVal = sys_fork(); //In task.c
	        break;

        case SYS_getc:
	        retVal = do_getc();
	        break;

        case SYS_puts:
	        do_puts((char*)a1, a2);
	        retVal = 0;
	        break;

        case SYS_getpid:
	        // Lab4: get current task's pid 
	        retVal = cur_task->task_id;
	        break;

        case SYS_sleep:
	        // Lab4: yield this task 
	        if (cur_task != NULL)
	        {
		        cur_task->remind_ticks = a1;
		        cur_task->state = TASK_SLEEP;
		        sched_yield();
	        }
	        retVal = 0;
	        break;

        case SYS_kill:
	        // Lab4: kill task 
	        sys_kill(a1);
	        retVal = 0;
	        break;

        case SYS_get_num_free_page:
            retVal = sys_get_num_free_page();
            break;

        case SYS_get_num_used_page:
            retVal = sys_get_num_used_page();
            break;

        case SYS_get_ticks:
            retVal = sys_get_ticks();
            break;

        case SYS_settextcolor:
            sys_settextcolor(a1,a2);
            retVal = 0;
            break;

        case SYS_cls:
            sys_cls(); 
            retVal = 0;
            break;
        
        // TODO: Lab7 file I/O system call
        case SYS_open:
        case SYS_read:    
        case SYS_write:
        case SYS_close:   
        case SYS_lseek:
        case SYS_unlink:
            retVal = -1; //Not yet implemented
            break;

	    }
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
	}
	    
	return retVal;
}

static void syscall_handler(struct Trapframe *tf)
{
//<<<<<<< HEAD
	/* TODO: Lab5
   * call do_syscall
   * Please remember to fill in the return value
   * HINT: You have to know where to put the return value
   */
	struct PushRegs r = tf->tf_regs;
	tf->tf_regs.reg_eax = do_syscall(r.reg_eax, r.reg_edx, r.reg_ecx, r.reg_ebx, r.reg_edi, r.reg_esi);
/*
=======
	// Lab4: call do_syscall 
	int32_t ret = -1;
	ret = do_syscall(tf->tf_regs.reg_eax, tf->tf_regs.reg_edx, tf->tf_regs.reg_ecx, tf->tf_regs.reg_ebx, tf->tf_regs.reg_edi, tf->tf_regs.reg_esi);	
	// Set system return value 
	tf->tf_regs.reg_eax = ret;

>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
}

void syscall_init()
{
//<<<<<<< HEAD
  /* TODO: Lab5
   * Please set gate of system call into IDT
   * You can leverage the API register_handler in kernel/trap.c
   */
	extern void SYS_Input();
	register_handler(T_SYSCALL, syscall_handler, SYS_Input, 1, 3);
/*
=======
	// Initial syscall trap after trap_init()
	// Register trap handler 
	extern void SYS_ISR();
	register_handler( T_SYSCALL, &syscall_handler, &SYS_ISR, 1, 3);
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
}

