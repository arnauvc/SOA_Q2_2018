#include <asm.h>
ENTRY(write)

	mov 4, %EAX
	sysenter $0x80
	push %ecx
	push %edx
	push %esp
	push %eip
	push %ebp
	cmp %eax, $0
	sysenter $0x80
	jge return
	mul -1, %eax
	mov
return:
	pop %ebp
	pop %eip
	pop %esp
	pop %edx
	pop %ecx
	sysexit
