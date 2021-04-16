#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#define __NR_helloworld 337
int main(int argc, char *argv[])
{
	syscall(__NR_helloworld);
	return 0;
}
