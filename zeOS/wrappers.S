#include <asm.h>
#include <errno.h>

ENTRY(write)
	push %ebp
	mov %esp, %ebp

	push %ebx
	push %esi

	mov 8(%ebp), %ebx
	mov 12(%ebp), %ecx
	mov 16(%ebp), %edx
	
	mov $4, %EAX
	
	push %ecx
	push %edx
	lea return1, %esi
	push %esi
	push %ebp
	mov %esp, %ebp
	sysenter

return1:
	pop %ebp
	pop %esi
	pop %edx
	pop %ecx
	
	cmpl $0, %eax
	jge fi

err:
	neg %eax
	mov %eax, errno
	mov $(-1), %eax

fi:
	pop %esi
	pop %ebx

	pop %ebp
	ret
	


ENTRY(gettime)
	mov 10, %eax
	sysenter
	cmpl $0, %eax
	jge return3
	movl $-ENOSYS, %eax
return3:
	ret
	#sysexit

		
