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
 push %ebx
 mov %esp, %ebp
 mov 16(%ebp), %edx
 mov 12(%ebp), %ecx
 mov 8(%ebp), %ebx
 mov 4, %EAX
 sysenter
 cmpl $0, %eax
 jge return2
 movl $-38, %eax

return2:
 mov %ebp, %esp
 pop %ebx
 pop %ebp
 sysexit


.globl gettime; .type gettime, @function; .align 0; gettime:
 mov 10, %eax
 sysenter
 cmpl $0, %eax
 jge return1
 movl $-38, %eax
return1:
 sysexit
