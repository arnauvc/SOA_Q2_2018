/*
 * entry.S - Entry point to system mode from user mode
 */

#include <asm.h>
#include <segment.h>
#include <errno.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %ebx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %edx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %edx; \
      pushl %ecx; \
      pushl %ebx; \
      movl $__KERNEL_DS, %edx;    \
      movl %edx, %ds;           \
      movl %edx, %es


#define RESTORE_ALL \
      popl %ebx; \
      popl %ecx; \
      popl %edx; \
      popl %esi; \
      popl %edi; \
      popl %ebp; \
      popl %eax; \
      popl %ds; \
      popl %es; \
      popl %fs; \
      popl %gs;


 #define EOI \
      movb $0x20, %al; \
      outb %al, $0x20;



ENTRY(keyboard_handler)
      SAVE_ALL
      call keyboard_routine;
      RESTORE_ALL
      EOI
      iret;

ENTRY(clock_handler)
    SAVE_ALL
    EOI
    call clock_routine;
    RESTORE_ALL
    iret;

ENTRY(system_call_handler)
    SAVE_ALL; \
    cmpl $0, %eax
    jl err
    cmpl $MAX_SYSCALL, %eax
    jg err
    call *sys_call_table(, %eax, 0x04); \
    jmp fin
err:
    movl $-ENOSYS, %eax
fin:
    movl %EAX, 0x18(%esp)
    RESTORE_ALL; \
    iret;


ENTRY(syscall_handler_sysenter)
    push $__USER_DS
    push %EBP
    pushfl
    push $__USER_CS
    push 4(%EBP)
    SAVE_ALL
    cmpl $0, %EAX
    jl sysenter_err
    cmpl $MAX_SYSCALL, %EAX
    jg sysenter_err
    call *sys_call_table(, %EAX, 0x04)
    jmp sysenter_fin
sysenter_err:
    movl $-ENOSYS, %EAX
sysenter_fin:
    movl %EAX, 0x18(%ESP)
    RESTORE_ALL
    movl (%ESP), %EDX
    movl 12(%ESP), %ECX
    sti
    sysexit


ENTRY(write_msr)
    push %ebp
    mov %esp, %ebp

    mov $0, %edx
    mov 8(%ebp), %eax
    mov 12(%ebp), %ecx
    wrmsr

    pop %ebp
    ret

ENTRY(task_switch)
  push %ebp
  mov %esp, %ebp

  push %esi
  push %edi
  push %ebx

  #push new
  push 8(%ebp)

  call inner_task_switch

  add $4, %esp ##Desempila el new = desmpila el ebx d'abans

  pop %ebx
  pop %edi
  pop %esi


  pop %ebp
  ret

ENTRY(inn_task_switch)
  
  #Agafem el punter al current()->kernel_esp
  mov 4(%esp), %eax
  
  #Accedim al valor apuntat per el current()->kernel_esp per donar-li el valor de ebp
  mov %ebp ,(%eax)
  
  #Modifiquem el nostre %esp, amb el new()->kernel_esp
  mov 8(%esp), %esp
  
  #En principi aqui ja estem al nou proces. Fem pop del ebp que en el cas del idle es 0, i saltem a cpu_idle.
  pop %ebp
  ret
