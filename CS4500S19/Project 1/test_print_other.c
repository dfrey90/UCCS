#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define __NR_print_other 339
#define BUFSIZE 128

int main(int argc, char *argv[])
{
	FILE *fp;	//var to hold capture output from 'pgrep bash'
	char cmd_output[BUFSIZE];	//var to grab info from fp(above)
	char *cmd = "pgrep bash";	//var to hold cmd to get arbitrary pid
	int pid;	//var that hold pid to give to syscall

	//set fp to output of cmd
	fp = popen(cmd, "r");
	
	//get line from fp, store into cmd_output
	fgets(cmd_output, BUFSIZE, fp);
	
	//close fp
	pclose(fp);

	//turn char str cmd_output  into int for syscall
	pid = atoi(cmd_output);

	//test pid successfully changed to same bash pid int
	printf("PID sent to print_other: %d\n", pid);

	//call syscall for print other with pid as arg
	syscall(__NR_print_other, pid);

	return 0;
}
