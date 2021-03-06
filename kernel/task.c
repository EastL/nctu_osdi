#include <inc/mmu.h>
#include <inc/types.h>
#include <inc/string.h>
#include <inc/x86.h>
#include <inc/memlayout.h>
#include <kernel/task.h>
#include <kernel/mem.h>
#include <kernel/cpu.h>
#include <kernel/spinlock.h>

// Global descriptor table.
//
// Set up global descriptor table (GDT) with separate segments for
// kernel mode and user mode.  Segments serve many purposes on the x86.
// We don't use any of their memory-mapping capabilities, but we need
// them to switch privilege levels. 
//
// The kernel and user segments are identical except for the DPL.
// To load the SS register, the CPL must equal the DPL.  Thus,
// we must duplicate the segments for the user and the kernel.
//
// In particular, the last argument to the SEG macro used in the
// definition of gdt specifies the Descriptor Privilege Level (DPL)
// of that descriptor: 0 for kernel and 3 for user.
//
struct Segdesc gdt[NCPU + 5] =
{
	// 0x0 - unused (always faults -- for trapping NULL far pointers)
	SEG_NULL,

	// 0x8 - kernel code segment
	[GD_KT >> 3] = SEG(STA_X | STA_R, 0x0, 0xffffffff, 0),

	// 0x10 - kernel data segment
	[GD_KD >> 3] = SEG(STA_W, 0x0, 0xffffffff, 0),

	// 0x18 - user code segment
	[GD_UT >> 3] = SEG(STA_X | STA_R, 0x0, 0xffffffff, 3),

	// 0x20 - user data segment
	[GD_UD >> 3] = SEG(STA_W , 0x0, 0xffffffff, 3),

	// First TSS descriptors (starting from GD_TSS0) are initialized
	// in task_init()
	[GD_TSS0 >> 3] = SEG_NULL
	
};

struct Pseudodesc gdt_pd = {
	sizeof(gdt) - 1, (unsigned long) gdt
};



//<<<<<<< HEAD
//static struct tss_struct tss;
//=======
//static struct Taskstate tss;
//>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
Task tasks[NR_TASKS];

extern char bootstack[];

extern char UTEXT_start[], UTEXT_end[];
extern char UDATA_start[], UDATA_end[];
extern char UBSS_start[], UBSS_end[];
extern char URODATA_start[], URODATA_end[];
/* Initialized by task_init */
uint32_t UTEXT_SZ;
uint32_t UDATA_SZ;
uint32_t UBSS_SZ;
uint32_t URODATA_SZ;
struct spinlock ave_lock;
unsigned int aver = 0; 

//Task *cur_task = NULL; //Current running task

extern void sched_yield(void);


//<<<<<<< HEAD
/* TODO: Lab5
 * 1. Find a free task structure for the new task,
 *    the global task list is in the array "tasks".
 *    You should find task that is in the state "TASK_FREE"
 *    If cannot find one, return -1.
 *
 * 2. Setup the page directory for the new task
 *
 * 3. Setup the user stack for the new task, you can use
 *    page_alloc() and page_insert(), noted that the va
 *    of user stack is started at USTACKTOP and grows down
 *    to USR_STACK_SIZE, remember that the permission of
 *    those pages should include PTE_U
 *
 * 4. Setup the Trapframe for the new task
 *    We've done this for you, please make sure you
 *    understand the code.
 *
 * 5. Setup the task related data structure
 *    You should fill in task_id, state, parent_id,
 *    and its schedule time quantum (remind_ticks).
 *
 * 6. Return the pid of the newly created task.
 
 */
struct spinlock task_lock;
//=======
//Find avaiable task slot and setup 
//>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
int task_create()
{
  struct PageInfo *p;
	Task *ts = NULL;

	/* Find a free task structure */
//<<<<<<< HEAD
	int i;
	spin_lock(&task_lock);
	for (i = 0; i < NR_TASKS; i++) {
		if (tasks[i].state == TASK_FREE) {
			ts = &tasks[i];
			break;
		}

		if (i == NR_TASKS - 1)
			return -1;
	}
	spin_unlock(&task_lock);
/*
=======
	int i = 0, j = 0;
	Task *ts = NULL;
	for (i = 0; i < NR_TASKS; i++)
	{
		if (tasks[i].state == TASK_FREE || tasks[i].state == TASK_STOP)
		{
			ts = &(tasks[i]);
			break;
		}
	}
	if (i >= NR_TASKS)
		return -1;

  // Setup Page Directory 
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/

  if (!(ts->pgdir = setupkvm()))
    panic("Not enough memory for per process page directory!\n");

//<<<<<<< HEAD
  /* Setup User Stack */
	int insert, index;
	struct PageInfo *newStack;
	for (index = 1; index < 11; index++) {
		newStack = page_alloc(0);
		if (page_insert(tasks[i].pgdir, newStack, (void *)(USTACKTOP-PGSIZE*index), (PTE_U | PTE_W)) != 0) {
//			panic("can't insert page");
			return -1;
		}
	}
//=======
//>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
	/* Setup Trapframe */
	memset( &(ts->tf), 0, sizeof(ts->tf));

	ts->tf.tf_cs = GD_UT | 0x03;
	ts->tf.tf_ds = GD_UD | 0x03;
	ts->tf.tf_es = GD_UD | 0x03;
	ts->tf.tf_ss = GD_UD | 0x03;
	ts->tf.tf_esp = USTACKTOP-PGSIZE;

  /* Setup User Stack 
	int j;
  for (j = 0; j < USR_STACK_SIZE; j += PGSIZE)
  {
    p = page_alloc(ALLOC_ZERO);
    if (!p || page_insert(ts->pgdir, p, (void*)(USTACKTOP-USR_STACK_SIZE+j), PTE_W|PTE_U))
      panic("Not enough memory for user stack!\n");
  }
*/
	
	/* Setup task structure (task_id and parent_id) */
	ts->task_id = i;
//<<<<<<< HEAD
	//ts->parent_id = 0;
	ts->parent_id = ((uint32_t)thiscpu->cpu_task)?thiscpu->cpu_task->task_id:0;
/*
=======
	ts->state = TASK_RUNNABLE;
	ts->parent_id = ((uint32_t)cur_task)?cur_task->task_id:0;
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
	ts->remind_ticks = TIME_QUANT;
	return i;
}

//<<<<<<< HEAD

/* TODO: Lab5
 * This function free the memory allocated by kernel.
 *
 * 1. Be sure to change the page directory to kernel's page
 *    directory to avoid page fault when removing the page
 *    table entry.
 *    You can change the current directory with lcr3 provided
 *    in inc/x86.h
 *
 * 2. You have to remove pages of USER STACK
 *
 * 3. You have to remove pages of page table
 *
 * 4. You have to remove pages of page directory
 *
 * HINT: You can refer to page_remove, ptable_remove, and pgdir_remove
 */


static void task_free(int pid)
{
	//use kernel pgdir
	lcr3(PADDR(kern_pgdir));


	//remove user stack
	int i;
	for (i = 1; i < 11 ; i++)
		page_remove(tasks[pid].pgdir, (void *)(USTACKTOP-PGSIZE*i));

	//printk("yo\n");
	ptable_remove(tasks[pid].pgdir);
	pgdir_remove(tasks[pid].pgdir);
/*
=======
// Free task of pid 
static void task_free(int pid)
{
  lcr3(PADDR(kern_pgdir));
  unsigned i;
  // Free pages of user stack 
  for (i = 0; i < USR_STACK_SIZE; i += PGSIZE)
    page_remove(tasks[pid].pgdir, (void*)(USTACKTOP-USR_STACK_SIZE+i));
  // Free page tables 
  ptable_remove(tasks[pid].pgdir);
  // Free page directory 
  pgdir_remove(tasks[pid].pgdir);
}

void task_switch(int id)
{
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
}

// Lab6 TODO
//
// Modify it so that the task will be removed form cpu runqueue
// ( we not implement signal yet so do not try to kill process
// running on other cpu )
//
void sys_kill(int pid)
{
//<<<<<<< HEAD
	int cpuID = tasks[pid].cpu_id;
	struct CpuInfo task_cpu = cpus[cpuID];
	if (pid > 0 && pid < NR_TASKS && task_cpu.cpu_rq.total != 0)
	{

		/* TODO: Lab 5
		 * Remember to change the state of tasks
		 * Free the memory
		 * and invoke the scheduler for yield
		 */
/*
		Task *current_task = task_cpu.cpu_rq.runq;

		int i ;
		for (i = 0; i < task_cpu.cpu_rq.total; i++) {
			if (current_task != NULL) {
				if (i == 0) {
					if (current_task->task_id == pid) {
						task_cpu.cpu_rq.runq = current_task->next;
						current_task->next = NULL;
						break;
					}
				}
				if (current_task->next != NULL) {
					if (current_task->next->task_id == pid) {
						current_task->next = current_task->next->next;
						current_task->next->next = NULL;
						break;
					}
				}
				current_task = current_task->next;
			}
		}
		task_cpu.cpu_rq.total -= 1;
*/
		if (thiscpu->cpu_rq.total <= 1)
			panic("there is no task in runq.\n");
		int i;
		for (i = 0; i < thiscpu->cpu_rq.total; i++)
		{
			if (thiscpu->cpu_rq.runq[i] == pid)
			{
				thiscpu->cpu_rq.runq[i] = thiscpu->cpu_rq.runq[thiscpu->cpu_rq.total - 1];
				break;
			}
		}

		thiscpu->cpu_rq.total -= 1;

		//change the state of tasks
		tasks[pid].state = TASK_FREE;
		//free memory
		task_free(pid);

		//invoke yield
		if (thiscpu->cpu_task->task_id == pid)
		{
			thiscpu->cpu_task = NULL;
			sched_yield();
		}
	}
}

/* TODO: Lab 5
 * In this function, you have several things todo
 *
 * 1. Use task_create() to create an empty task, return -1
 *    if cannot create a new one.
 *
 * 2. Copy the trap frame of the parent to the child
 *
 * 3. Copy the content of the old stack to the new one,
 *    you can use memcpy to do the job. Remember all the
 *    address you use should be virtual address.
 *
 * 4. Setup virtual memory mapping of the user prgram 
 *    in the new task's page table.
 *    According to linker script, you can determine where
 *    is the user program. We've done this part for you,
 *    but you should understand how it works.
 *
 * 5. The very important step is to let child and 
 *    parent be distinguishable!
 *
 * HINT: You should understand how system call return
 * it's return value.
 */

//
// Lab6 TODO:
//
// Modify it so that the task will disptach to different cpu runqueue
// (please try to load balance, don't put all task into one cpu)
//
int sys_fork()
{
	/* pid for newly created process */
	pte_t *pte;
	int pid, i;
	struct PageInfo *pp;
	pid = task_create();
	if (pid == -1)
		return pid;
	tasks[pid].tf = thiscpu->cpu_task->tf;

	//Copy the content of the old stack to the new one
	for (i = 0; i < 10; i++)
	{
		pp = page_lookup(thiscpu->cpu_task->pgdir, USTACKTOP-PGSIZE*(10-i), &pte);
		lcr3(PADDR(tasks[pid].pgdir));
		if(page_insert(tasks[pid].pgdir, pp, (void *)(UTEMP+PGSIZE*i), PTE_U) != 0)
		{
			printk("insert temp memory error!!\n");
			return -1;
		}
		memcpy(USTACKTOP-PGSIZE*(10-i), UTEMP+PGSIZE*i, PGSIZE);
		page_remove(tasks[pid].pgdir, (void *)(UTEMP+PGSIZE*i));
		lcr3(PADDR(thiscpu->cpu_task->pgdir));
	}

	if ((uint32_t)thiscpu->cpu_task)
	{
		/* Step 4: All user program use the same code for now */
		setupvm(tasks[pid].pgdir, (uint32_t)UTEXT_start, UTEXT_SZ);
		setupvm(tasks[pid].pgdir, (uint32_t)UDATA_start, UDATA_SZ);
		setupvm(tasks[pid].pgdir, (uint32_t)UBSS_start, UBSS_SZ);
		setupvm(tasks[pid].pgdir, (uint32_t)URODATA_start, URODATA_SZ);
	}

	tasks[pid].state = TASK_RUNNABLE;
	tasks[pid].tf.tf_regs.reg_eax = 0;

	struct CpuInfo *nextcpu;//use to average task
	//nextcpu = &cpus[(thiscpu->cpu_id + 1) % ncpu];
	//printk("averageaaa\n");
	spin_lock(&ave_lock);
	nextcpu = &cpus[(++aver) % ncpu];
	spin_unlock(&ave_lock);
	//printk("average:%d\n", aver%ncpu);

	//printk("testA\n");
	if (nextcpu->cpu_rq.total >= NR_TASKS)
		panic("runq is full!\n");
	//printk("testB\n");

	nextcpu->cpu_rq.runq[nextcpu->cpu_rq.total] = pid;
	nextcpu->cpu_rq.total += 1;
	//printk("%d ", nextcpu->cpu_id);
	//printk("%d\n", nextcpu->cpu_rq.total);
	
	return pid;
}
/*
=======
	// Lab4: Died task recycele, just set task state as TASK_STOP and yield 
  
	if (pid > 0 && pid < NR_TASKS)
	{
		tasks[pid].state = TASK_STOP;
    task_free(pid);
		sched_yield();
	}
}
void sys_kill_self()
{
	//Lab4: Died task recycele 
	if ((uint32_t)cur_task)
	{
		cur_task->state = TASK_STOP;
    task_free(cur_task->task_id);
		sched_yield();
	}
}

int sys_fork()
{
	int pid = -1, i;
  uint32_t pa_src, pa_dst, *pte_src, *pte_dst;
	// Setup task space 
	pid = task_create();
	if (pid < 0 )
		return -1;
	
	if ((uint32_t)cur_task)
	{
		// Lab4: Copy parent tf to new task 
		tasks[pid].tf = cur_task->tf;
	
		// Lab4: Copy old usr_stack to new task 
    for(i = 0; i < USR_STACK_SIZE; i += PGSIZE)
    {
      if((pte_src = pgdir_walk(cur_task->pgdir, (void *)(USTACKTOP-USR_STACK_SIZE+i), 0)) == 0)
        panic("copy user stack: pte_src should exist");
      if(!(*pte_src & PTE_P))
        panic("copy user stack: page not present");
      pa_src = PTE_ADDR(*pte_src);

      if((pte_dst = pgdir_walk(tasks[pid].pgdir, (void *)(USTACKTOP-USR_STACK_SIZE+i), 0)) == 0)
        panic("copy user stack: pte_dst should exist");
      if(!(*pte_dst & PTE_P))
        panic("copy user stack: page not present");
      pa_dst = PTE_ADDR(*pte_dst);

      memmove((char*)KADDR(pa_dst), (char*)KADDR(pa_src), PGSIZE);
    }

    // All user program use the same code for now 
    setupuvm(tasks[pid].pgdir, (uint32_t)UTEXT_start, UTEXT_SZ);
    setupuvm(tasks[pid].pgdir, (uint32_t)UDATA_start, UDATA_SZ);
    setupuvm(tasks[pid].pgdir, (uint32_t)UBSS_start, UBSS_SZ);
    setupuvm(tasks[pid].pgdir, (uint32_t)URODATA_start, URODATA_SZ);

		// Lab4: Set system call's return value 
		cur_task->tf.tf_regs.reg_eax = pid;
		tasks[pid].tf.tf_regs.reg_eax = 0;
	}
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0

	return pid;
}
*/
void task_init()
{
  extern int user_entry();
	int i;
  UTEXT_SZ = (uint32_t)(UTEXT_end - UTEXT_start);
  UDATA_SZ = (uint32_t)(UDATA_end - UDATA_start);
  UBSS_SZ = (uint32_t)(UBSS_end - UBSS_start);
  URODATA_SZ = (uint32_t)(URODATA_end - URODATA_start);
  /*printk("UText start:[%p], UText end:[%p]\n", UTEXT_start, UTEXT_end);*/
  /*printk("UData start:[%p], UData end:[%p]\n", UDATA_start, UDATA_end);*/
  /*printk("UBss start:[%p], UBss end:[%p]\n", UBSS_start, UBSS_end);*/
  /*printk("URODATA start:[%p], URODATA end:[%p]\n", URODATA_start, URODATA_end);*/

	/* Initial task sturcture */
	for (i = 0; i < NR_TASKS; i++)
	{
		memset(&(tasks[i]), 0, sizeof(Task));
		tasks[i].state = TASK_FREE;

	}
	task_init_percpu();
	spin_initlock(&ave_lock);
	spin_initlock(&task_lock);
}

// Lab6 TODO
//
// Please modify this function to:
//
// 1. init idle task for non-booting AP 
//    (remember to put the task in cpu runqueue) 
//
// 2. init per-CPU Runqueue
//
// 3. init per-CPU system registers
//
// 4. init per-CPU TSS
//
void task_init_percpu()
{
	

	int i;
	extern int user_entry();
	extern int idle_entry();
	
	// Setup a TSS so that we get the right stack
	// when we trap to the kernel.
	memset(&(thiscpu->cpu_tss), 0, sizeof(thiscpu->cpu_tss));
	thiscpu->cpu_tss.ts_esp0 = (uintptr_t)percpu_kstacks[thiscpu->cpu_id] + KSTKSIZE;
	thiscpu->cpu_tss.ts_ss0 = GD_KD;

	// fs and gs stay in user data segment
	thiscpu->cpu_tss.ts_fs = GD_UD | 0x03;
	thiscpu->cpu_tss.ts_gs = GD_UD | 0x03;

	/* Setup TSS in GDT */
//<<<<<<< HEAD
	gdt[(GD_TSS0 >> 3) + thiscpu->cpu_id] = SEG16(STS_T32A, (uint32_t)(&(thiscpu->cpu_tss)), sizeof(struct tss_struct) - 1, 0);
	gdt[(GD_TSS0 >> 3) + thiscpu->cpu_id].sd_s = 0;

	/* Setup first task */
	i = task_create();
	thiscpu->cpu_task = &(tasks[i]);

	/* For user program */
	setupvm(thiscpu->cpu_task->pgdir, (uint32_t)UTEXT_start, UTEXT_SZ);
	setupvm(thiscpu->cpu_task->pgdir, (uint32_t)UDATA_start, UDATA_SZ);
	setupvm(thiscpu->cpu_task->pgdir, (uint32_t)UBSS_start, UBSS_SZ);
	setupvm(thiscpu->cpu_task->pgdir, (uint32_t)URODATA_start, URODATA_SZ);
	if (bootcpu->cpu_id == thiscpu->cpu_id) {
		thiscpu->cpu_task->tf.tf_eip = (uint32_t)user_entry;
		thiscpu->cpu_task->state = TASK_RUNNING;
	} else {
		thiscpu->cpu_task->tf.tf_eip = (uint32_t)idle_entry;
		thiscpu->cpu_task->state = TASK_IDLE;
	}

	/* Setup run queue */
	memset(&(thiscpu->cpu_rq), 0, sizeof(thiscpu->cpu_rq));
	thiscpu->cpu_rq.runq[0] = i;
	thiscpu->cpu_rq.current_index = 0;
	thiscpu->cpu_rq.total = 1;
/*
=======
	gdt[GD_TSS0 >> 3] = SEG16(STS_T32A, (uint32_t)(&tss), sizeof(struct Taskstate), 0);
	gdt[GD_TSS0 >> 3].sd_s = 0;

	// Setup first task 
	i = task_create();
	cur_task = &(tasks[i]);

  //For user program 
  setupuvm(cur_task->pgdir, (uint32_t)UTEXT_start, UTEXT_SZ);
  setupuvm(cur_task->pgdir, (uint32_t)UDATA_start, UDATA_SZ);
  setupuvm(cur_task->pgdir, (uint32_t)UBSS_start, UBSS_SZ);
  setupuvm(cur_task->pgdir, (uint32_t)URODATA_start, URODATA_SZ);
  cur_task->tf.tf_eip = (uint32_t)user_entry;
>>>>>>> a34fb2bb00b319f01d75d07a1c27561390a0eea0
*/
	
	/* Load GDT&LDT */
	lgdt(&gdt_pd);
/*
	asm volatile("movw %%ax, %%gs" :: "a" (GD_UD|3));
	asm volatile("movw %%ax, %%fs" :: "a" (GD_UD|3));

	asm volatile("movw %%ax, %%es" :: "a" (GD_KD));
	asm volatile("movw %%ax, %%ds" :: "a" (GD_KD));
	asm volatile("movw %%ax, %%ss" :: "a" (GD_KD));

	asm volatile("ljmp %0, $1f\n 1:\n" :: "i" (GD_KT));
*/



	lldt(0);

	// Load the TSS selector 
	ltr((GD_TSS0 & 0x3) | (((GD_TSS0 >> 3) + thiscpu->cpu_id) << 3) );

	//cur_task->state = TASK_RUNNING;
}
