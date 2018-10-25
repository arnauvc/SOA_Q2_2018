/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

int write_msr(int val_msr, int num_msr);
void task_switch(union task_union *new);
void inn_task_switch(unsigned long current_esp, unsigned long  new_esp);


struct list_head freequeue; // Cua de task_structs lliures
struct list_head readyqueue; // Cua de task_structs(processos) en ready per entrar a la CPU

struct task_struct * idle_task;

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

struct task_struct *list_head_to_task_struct(struct list_head *l){

	return (struct task_struct *)((unsigned long)l&0xfffff000);
}

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

void inner_task_switch(union task_union *new) {

	// Changing user adress space
 	//set_cr3(new->task.dir_pages_baseAddr);

  //  Update the pointer to the system stack to point to the stack of new_task.
  tss.esp0 = (unsigned long) &new->stack[KERNEL_STACK_SIZE];
  // Update MSR number 0x175
  write_msr(tss.esp0,0x175);

 	// Changing user adress space
  set_cr3(new->task.dir_pages_baseAddr); 


  //new.task //task_struct del nou process
  struct task_struct* new_task = (struct task_struct*) new;
  //current() //task_struct del process actual
  struct task_struct* current_task = current();

  //inn_task_switch( &(current_task->kernel_esp), &(new_task->kernel_esp) ); // Perque passem direccions en comptes de valors??
   inn_task_switch( current_task->kernel_esp, new_task->kernel_esp );
}


void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}

}

void init_idle (void) {


  struct list_head * idle_list_head = list_first( &freequeue );
  list_del(idle_list_head);
  struct task_struct * idle_task_struct = list_head_to_task_struct(idle_list_head);
  union task_union *union_task = (union task_union*)idle_task_struct;

   idle_task_struct->PID = 0;
   ///////////////////////////////////////////
   int allocate_result = allocate_DIR(idle_task_struct); //dir_pages_baseAddr


   //inicialitzar context execucio
   union_task->stack[KERNEL_STACK_SIZE-1] = (unsigned long) &cpu_idle; // @adreça retorn
   union_task->stack[KERNEL_STACK_SIZE-2] = 0; //ebp (no importa el valor)
   union_task->task.kernel_esp =(unsigned long) &(union_task->stack[KERNEL_STACK_SIZE-2]);

   //Initialize global variable idle_task
   //idle_task =  idle_task_struct;
   idle_task = (struct task_struct *)union_task;


}

void init_task1(void) {
   struct list_head * task1_list_head = list_first( &freequeue );
   list_del(task1_list_head);
   struct task_struct * task1_task_struct = list_head_to_task_struct(task1_list_head);
   task1_task_struct->PID = 1;
   ///////////////////////////////////////////
   int allocate_result = allocate_DIR(task1_task_struct); //dir_pages_baseAddr
   page_table_entry * dir_task1 = get_DIR(task1_task_struct);

   set_user_pages( task1_task_struct );
   // Creation of Task Union
   union task_union * init_union = (union task_union *) task1_task_struct;

   //////////////////////////////////////////
   // Update of TSS;
   tss.esp0 = (unsigned long) &(init_union->stack[KERNEL_STACK_SIZE]);

   // Modify MSR number 0x175
   write_msr((unsigned long) &(init_union->stack[KERNEL_STACK_SIZE]),0x175); ///////////

   set_cr3(dir_task1);

}


void init_sched() {
	INIT_LIST_HEAD( &freequeue );
	INIT_LIST_HEAD( &readyqueue );

	//Afegirx tots els task_structs a la freequeue;
	for (int i = 0; i < NR_TASKS; ++i) {
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
