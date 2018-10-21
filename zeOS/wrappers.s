# 1 "wrappers.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "wrappers.S"
# 1 "include/asm.h" 1
# 2 "wrappers.S" 2
# 1 "include/errno.h" 1
# 3 "wrappers.S" 2

.globl write; .type write, @function; .align 0; write:
 push %ebp
 mov %ebp, %esp

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



.globl gettime; .type gettime, @function; .align 0; gettime:
 mov 10, %eax
 sysenter
 cmpl $0, %eax
 jge return3
 movl $-38, %eax
return3:
 ret
 #sysexit
