#include <asm.h>
#include <errno.h>

ENTRY(write)
	push %ebp
	push %ebx
	mov %esp, %ebp
	mov 16(%ebp), %edx
	mov 12(%ebp), %ecx
	mov 8(%ebp), %ebx  
	mov 4, %EAX
	sysenter
	cmpl $0, %eax
	jge return2
	movl $-ENOSYS, %eax
	
return2:
	mov %ebp, %esp
	pop %ebx
	pop %ebp
	sysexit


ENTRY(gettime)
	mov 10, %eax
	sysenter
	cmpl $0, %eax
	jge return1
	movl $-ENOSYS, %eax
return1:
	sysexit

		
