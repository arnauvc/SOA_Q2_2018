/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>


struct list_head freequeue; // Cua de task_structs lliures
struct list_head readyqueue; // Cua de task_structs(processos) en ready per entrar a la CPU

struct task_struct * idle_task;

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


//#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
//#endif

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void){
	 struct list_head * idle_list_head = list_first( &freequeue );
	 struct task_struct * idle_task_struct = list_head_to_task_struct(idle_list_head);
	 idle_task_struct->PID = 0;
	 int result_allocate = allocate_DIR(idle_task_struct); //dir_pages_baseAddr 

	 //inicialitzar context execucio
	 task[0].stack[KERNEL_STACK_SIZE-1] = (int)*cpu_idle; // @adreça retorn
	 task[0].stack[KERNEL_STACK_SIZE-2] = 0; //ebp (no importa el valor)
	 task[0].task.kernel_esp = & task[0].stack[KERNEL_STACK_SIZE-2];

	 //Initialize global variable idle_task
	 idle_task =  idle_task_struct;
}

void init_task1(void){


}


void init_sched(){
	INIT_LIST_HEAD( &freequeue );
	INIT_LIST_HEAD( &readyqueue );

	//Afegirx tots els task_structs a la freequeue;
	for(int i = 0; i < NR_TASKS; ++i){
		list_add( &(task[i].task.list), &freequeue );
	}
	

}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

