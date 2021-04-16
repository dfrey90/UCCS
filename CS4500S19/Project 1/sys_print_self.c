#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_print_self(void)
{
	//printk(KERN_EMERG "Self\n");

	//prints out current pid and program name
	printk(KERN_EMERG "Self PID: %d, Name: %s\n", current->pid, current->comm);

	//print parent pid and name until init
	//make struct pointer of task info
	struct task_struct *task;

	printk(KERN_EMERG "Parent Process until init:\n");

	//loop through each task, each iteration is parent until init
	for(task = current; task != &init_task; task = task->parent)
	{
		printk(KERN_EMERG "PID: %d, Name: %s\n", task->pid, task->comm);
	}

	return 0;
}

