#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/highmem.h>

asmlinkage int sys_vma_status(unsigned long addr, int pid)
{
	struct task_struct *task;	//task_struct from pid
	struct mm_struct *mem;	//mm from task_struct
	pgd_t *pgd;	//page global directory
	pud_t *pud;	//page upper directory
	pmd_t *pmd;	//page middle directory
	pte_t *ptep;	//page table entry pointer
	pte_t pte;	//page table entry
	spinlock_t *s_lock;	//spin lock to avoid kernel oops on pte
	int present;	//used to set if addr is in memory
	
	//get task_struct from pid
	task = find_task_by_vpid(pid);
	
	//get mm from task_struct
	mem = task->mm;
	
	//get page directories: must get in order pgd->pud->pmd->ptep->pte
	pgd = pgd_offset(mem, addr);
	pud = pud_offset(pgd, addr);
	pmd = pmd_offset(pud, addr);
	
	//apply spin lock to avoid kernel oops when getting pte from pmd
	ptep = pte_offset_map_lock(mem, pmd, addr, &s_lock);
	
	//get pte from ptep
	pte = *ptep;
	
	//figure out if in memory
	present = pte_present(pte);
	
	//unlock the lock
	pte_unmap_unlock(ptep, s_lock);
	
	//display if in memory or not
	printk(KERN_EMERG "Address: %lu, PID: %d, Present: %d\n", addr, pid, present);
	
	return 0;
}
