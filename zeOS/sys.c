/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1


extern int zeos_ticks;


int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

char bufferdest[4096]; 

int sys_write(int fd, char * buffer, int size) {
  /*
  Check user parameters
    -fd
    -buffer
    -size
  Copy data from/to user address space

  int sys_write_console (char *buffer, int size);

  return result
  */
	
	int comprovar = check_fd(fd,ESCRIPTURA);
	if (comprovar != 0) return -1;
	if (buffer == NULL) return -1;
	if (size <= 0) return -1;

	
	//char bufferdest[4096]; 
	int size_escriure = size;
	char * buffer_escriure = buffer;
	if(size > 4096){	
		while (size_escriure > 4096) {
			if (copy_from_user(buffer_escriure, bufferdest, 4096) != 0) return -1;
			if (sys_write_console(bufferdest, 4096) <= 0) return -1;
			size_escriure -= 4096;
			buffer_escriure = buffer_escriure + 4096;
		} 
	}
	else{
		if (copy_from_user(buffer, bufferdest, 4096) != 0) return -1;
		if (sys_write_console(bufferdest, size) <= 0) return -1;
	}

	return 0;
	


}


int sys_gettime() {
  return zeos_ticks;
}




int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process

  return PID;
}

void sys_exit()
{
}
