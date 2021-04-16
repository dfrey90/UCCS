#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/pid.h>

asmlinkage int sys_print_other(int pid)
{
	struct task_struct *task;	//struct for process struct
	
	//printk(KERN_EMERG "Other\n");

	//get task struct for specific process
	task = pid_task(find_vpid(pid), PIDTYPE_PID);

	//if task is null then no process, otherwise print info about process
	if (task != NULL)
	{
		printk(KERN_EMERG "PID: %d, name: %s\n", task->pid, task->comm);
	}
	else
	{
		printk(KERN_EMERG "PID: %d is not a valid process ID\n", pid);
	}

	return 0;
}

