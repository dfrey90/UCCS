#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#define __NR_print_self 338
int main(int argc, char *argv[])
{
	syscall(__NR_print_self);
	return 0;
}
