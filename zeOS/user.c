#include <libc.h>

char buffer[32] = "abc";

int __attribute__ ((__section__(".text.main")))
  main(void)
{
	
	write(1, buffer, 32);
	while (1);
	return 0;
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

    
  //while(1) { }
}
