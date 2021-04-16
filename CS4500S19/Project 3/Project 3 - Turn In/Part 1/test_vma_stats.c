#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define __NR_vma_stats 338
#define BUFSIZE 128

int main(int argc, char *argv[])
{
	FILE *fp;	//var to hold capture output from 'pgrep bash'
	char cmd_output[BUFSIZE];	//var to grab info from fp
	char *cmd = "pgrep bash";	//cmd to run to get pid
	int pid;	//var to hold pid from cmd to give to syscall

	//set fp to output of cmd
	fp = popen(cmd, "r");

	//get line from fp, store into cmd_output
	fgets(cmd_output, BUFSIZE, fp);
	
	//close fp
	pclose(fp);

	//turn char str of pid from cmd into int for syscall
	pid = atoi(cmd_output);

	//test pid sucessfully changed to int
	printf("PID sent to vma_stats: %d\n", pid);

	//call syscall for vma_stats of pid
	syscall(__NR_vma_stats, pid);

	return 0;
}
