#include <asm.h>
ENTRY(write)
	push %ebp
	push %ebx
	mov %esp, %ebp
	mov 16(%ebp), %edx
	mov 12(%ebp), %ecx
	mov 8(%ebp), %ebx  
	mov 4, %EAX
	sysenter $0x80
	cmp %eax, $0
	jge return
	movl $-ENOSYS, %eax
	jmp fi
return:
	mov %ebp, %esp
	pop %ebx
	pop %ebp
	sysexit
fi:



ENTRY(gettime)
	mov 10, %eax
	sysenter %0x80
	cmp %eax, $0
	jge return
	movl $-ENOSYS, %eax
return:
	sysexit

		
