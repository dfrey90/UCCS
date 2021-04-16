#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>

asmlinkage int sys_vma_stats(int pid)
{
	struct task_struct *task;	//task_struct from process
	struct mm_struct *mem;	//mm (memory) from task_struct
	struct vm_area_struct *vma;	//virtual memory area from mm_map
	int total_size = 0;	//var to hold total size of all vma
	
	//get task_struct from passed in process id
	task = find_task_by_vpid(pid);
	
	//get mm_struct from task_struct
	mem = task->mm;
	
	//loop through all vma of process adding each vma
	for (vma = mem->mmap; vma != NULL; vma = vma->vm_next)
	{
		//subtract start from end to get size of one vm and add to running total
		total_size += vma->vm_end - vma->vm_start;
	}
	
	//display total vma size of process
	printk(KERN_EMERG "PID %d total size VMA (bytes): %d, (KB): %d\n", task->pid, total_size, total_size*1024);

	return 0;
}
