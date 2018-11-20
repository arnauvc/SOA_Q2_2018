#include <libc.h>
#include <schedperf.h>

char buff[24];

int pid;


void clearScreen() {
	int i, j;
	write(1,"\n",1);
	for (i=0;i<33;++i) {
		for (j=0;j<80;++j) {
			write(1," ",1);
		}
	}
}

void WriteStats(int pid){

	struct stats st;
	get_stats(pid, &st);

	char buffer[4];
	write(1,"\n",1);
	write(1, "user_ticks:", 11);
	itoa(st.user_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "system_ticks:", strlen("system_ticks:"));
	itoa(st.system_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "blocked_ticks:", strlen("blocked_ticks:"));
	itoa(st.blocked_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "ready_ticks:", strlen("ready_ticks:"));
	itoa(st.ready_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "elapsed_total_ticks:", strlen("elapsed_total_ticks:"));
	itoa(st.elapsed_total_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "total_trans:", strlen("total_trans:"));
	itoa(st.blocked_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);

	write(1, "remaining_ticks:", strlen("remaining_ticks:"));
	itoa(st.blocked_ticks, buffer);
	write(1, buffer, strlen(buffer));
	write(1,"\n",1);
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	int estadistiques[3];
	clearScreen();
  	set_sched_policy(0); // 0 RR, 1 FIFO

  	int pid = fork();
  	
  	int vec[10];
	switch (pid){

		case 0 : //fill CPU
			write(1, "fill 1", strlen("fill 1"));
			//write(1,"\n",1);
  			int i, num = 1, primes = 0;
		    while (num <= 20000) { 
		        i = 2; 
		        while (i <= num) { 
		            if(num % i == 0)
		                break;
		            i++; 
		        }
		        if (i == num)
		            primes++;
		        num++;
		    }
		    //write(1,"\n",1);
			char buffer1[7];
			itoa(primes, buffer1);
			//write(1, "valor de primes:", strlen("valor de primes:"));
			//write(1, buffer1, strlen(buffer1));
			int gp = getpid();
			char buf[4];
			write(1, " / pid:", strlen("pid:"));
			itoa(gp, buf);
			write(1, buf, strlen(buf));
			//write(1,"\n",1);

			WriteStats(gp);
			exit();
		break;
 
		default : //default	
			
			estadistiques[0] = pid;
			
		break;
	}

	int gp1 = getpid();
			char bufe[4];
			write(1,"\n",1);
			write(1, "pid pare:", strlen("pid pare:"));
			itoa(gp1, bufe);
			write(1, bufe, strlen(bufe));
			write(1,"\n",1);
	
	int pid1 = fork();
	switch (pid1 ){

		case 0 : //fill I/O
			write(1, "fill 2", 6);
			for(int i = 0; i < 10000; ++i){
				vec[i%10] = i;
			}
			char buffe[4];
			itoa(vec[4], buffe);	
			write(1, buffe, strlen(buffe));
			int gp = getpid();
			char buf[4];
			write(1, "pid:", strlen("pid:"));
			itoa(gp, buf);
			write(1, buf, strlen(buf));
			write(1,"\n",1);

			WriteStats(gp);
			exit();
		break;

		default : //default	
			estadistiques[1] = pid1;
			WriteStats(getpid());
		break;
	}
/*
	int pid2 = fork();
	switch (pid2) {
		case 0 : //fill
			write(1, "fill 3", 32);
			int i, num = 1, primes = 0;
			while (num <= 1000) { 
		        i = 2; 
		        while (i <= num) { 
		        	for(int i = 0; i < 100; ++i){
						vec[i%10] = i;
					}
		            if(num % i == 0)
		                break;
		            i++; 
		        }
		        if (i == num)
		            primes++;
		        num++;
		    }
		    WriteStats(getpid());
			exit();
		break;

		default : //default	
			estadistiques[2] = pid2;
		break;
	}
	*/
	//wait pid
	/*
	char buffer[4];
	itoa(vec[4], buffer);
	write(1, buffer, 4);
*/

	//WriteStats(estadistiques[0]);
	//WriteStats(estadistiques[1]);

	//exit();
 	while(1) { }

}
