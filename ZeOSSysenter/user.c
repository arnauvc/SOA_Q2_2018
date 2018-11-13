#include <libc.h>
#include <schedperf.h>

char buff[24];

int pid;

void WriteStats(int pid){

	struct stats *st;
	get_stats(pid, st);

	char buffer[4];

	write(1, "user_ticks", 32);
	itoa(st->user_ticks, buffer);
	write(1, buffer, 4);

	itoa(st->system_ticks, buffer);
	write(1, buffer, 4);
	
	itoa(st->blocked_ticks, buffer);
	write(1, buffer, 4);

	itoa(st->ready_ticks, buffer);
	write(1, buffer, 4);

	itoa(st->elapsed_total_ticks, buffer);
	write(1, buffer, 4);

	itoa(st->total_trans, buffer);
	write(1, buffer, 4);

	itoa(st->remaining_ticks, buffer);
	write(1, buffer, 4);
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	int estadistiques[3];

  	set_sched_policy(0); // 0 RR, 1 FIFO

  	int pid = fork();
  	int a = 1;

	switch (pid){

		case 0 : //fill
			for(int i = 0; i < 1000000; ++i){
				a += a;
			}
			write(1, "fill 1", 32);
			exit();
		break;
 
		default : //default	
			estadistiques[0] = pid;
		break;
	}

	int pid1 = fork();
	switch (pid1 ){

		case 0 : //fill
			write(1, "fill 2", 32);
			exit();
		break;

		default : //default	
			estadistiques[1] = pid1;
		break;
	}

	int pid2 = fork();
	switch (pid2) {
		case 0 : //fill
			write(1, "fill 3", 32);
			exit();
		break;

		default : //default	
			estadistiques[2] = pid2;
		break;
	}
	//wait pid
	WriteStats(pid);


	
	

  while(1) { }

}
