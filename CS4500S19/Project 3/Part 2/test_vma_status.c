#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __NR_vma_status 339
#define BUFSIZE 128

int main(int argc, char *argv[])
{
	FILE *fp;	//var to hold capture output from 'pgrep bash'
	char cmd[BUFSIZE] = "pmap $(pgrep bash)";	//cmd to get mem addr from bash pid
	char pid_line[BUFSIZE];	//pid line as string
	char addr_line[BUFSIZE];	//mem addr line as string
	int pid;	//pid from cmd to give to syscall
	unsigned long addr;	//addr to find in memory

	//set fp to output of cmd
	fp = popen(cmd, "r");
	
	//get first line from fp with pid
	fgets(pid_line, BUFSIZE, fp);
	
	//get line from fp, store into cmd_output from cmd 'pgrep bash'
	fgets(addr_line, BUFSIZE, fp);
	
	//parse pid_line and convert to int
	pid = atoi(strtok(pid_line, ":"));
	
	//parse addr_line and convert to ul (16 = base 16)
	addr = strtoul(strtok(addr_line, " "), NULL, 16);

	//close fp
	pclose(fp);

	//test pid sucessfully changed to int
	printf("Sent to vma_status: Address: %lu, PID: %d\n", addr, pid);

	//call syscall for vma_stats of pid
	syscall(__NR_vma_status, addr, pid);

	return 0;
}
