/*
 * File: start.c
 *
 * Descri��o:
 *     Rotinas de execu��o de thread e processos.
 *     Faz parte do Process Manager, uma parte fundamental do Kernel Base.
 *
 * Vers�o 1.0, 2015, 2016. 
 */


#include <kernel.h>


//
// Fun��es importadas.
//

extern void turn_task_switch_on();


/*
 * KeStartIdle:
 *     Inicia a thead idle.
 *
 *     @todo: 
 *         + Iniciar o processo da idle.
 *         + Detectar se o aplicativo est� carregado na mem�ria 
 *	         antes de passar o comando pra ele via iret.
 *          Mudar para startIdleThread().
 *
 */
void KeStartIdle()
{
	int i;
	struct thread_d *Thread;
	
	Thread = (void *) threadList[0];
	if((void*) Thread == NULL)
	{
		//printf("KeStartIdle error: Struct!\n");
	    MessageBox(gui->screen,1,"ERRO","KeStartIdle: struct");
        KeAbort();
	}else{
	    
		//Checar se o programa j� foi inicializado antes. 
		//Ele n�o pode estar.
	    if(Thread->saved != 0){
	        printf("KeStartIdle error: Context!\n");
            KeAbort(); 			
	    };
	    
		//Checar se o slot na estrutura de tarefas � v�lido.
	    if(Thread->used != 1 || Thread->magic != 1234){
	        printf("KeStartIdle: Thread %d corrompida.\n", Thread->tid);
            KeAbort(); 		
	    };
		
        set_current(0);      //Seta a thread atual.	
		//...
	};
	
	// State ~ Checa o estado da tarefa.	 
    if(Thread->state != STANDBY){
        printf("KeStartIdle error: State. Id={%d}\n",Thread->tid);
	    refresh_screen();
	    while(1){};
    };
	
    // * MOVEMENT 2 ( Standby --> Running)	
    if(Thread->state == STANDBY){
		Thread->state = RUNNING;    
		queue_insert_data(queue, (unsigned long) Thread, QUEUE_RUNNING);
	};		
//Done!
done:	
	//Debug:
	//Alertando.
    //printf("* Starting idle TID=%d \n", Thread->tid); 
	//refresh_screen(); //@todo: isso � necess�rio ??


	for( i=0; i<=DISPATCHER_PRIORITY_MAX; i++){
	    dispatcherReadyList[i] = (unsigned long) IdleThread;
	};
	
    IncrementDispatcherCount(SELECT_IDLE_COUNT);
	
	/*
	    Gn 15: 9-17:
            " E as aves de rapina desciam sobre os cad�veres, 
			Abr�o, por�m, as enxotava. ".
			
			" Ele tomou o p�o, deu gra�as, e o partiu e 
			deu a seus disc�pulos, dizendo: 
			TOMAI, TODOS, E COMEI: 
			ISTO � O MEU CORPO, QUE SER� ENTREGUE POR V�S. ".
	*/
	
	/*
	 * turn_task_switch_on:
	 * + Cria um vetor para o timer, IRQ0.
	 * + Habilita o funcionamento do mecanismo de taskswitch.
	 */	 
	turn_task_switch_on(); 
	
	
	//
	// @todo: Testando timer novamente.
	//        *IMPORTANTE Me parece que tem que configurar o PIT por �ltimo.
	//
	
	timerInit8253(); 
	    		
    asm volatile(" cli \n"   
                 " mov $0x23, %ax  \n" 
                 " mov %ax, %ds  \n"
                 " mov %ax, %es  \n"
                 " mov %ax, %fs  \n"
                 " mov %ax, %gs  \n"
                 " pushl $0x23            \n"    //ss.
                 " movl $0x0044FFF0, %eax \n"
                 " pushl %eax             \n"    //esp.
                 " pushl $0x3200          \n"    //eflags.
                 " pushl $0x1B            \n"    //cs.
                 " pushl $0x00401000      \n"    //eip. 
                 " iret \n" );
				//Nothing.
// Fail ~ retorno inesperado.
fail:
    panic("KeStartIdle error: Return!"); 	
	while(1){};  //no return.	   
};


/*
 * KeStartShell:
 *     Executa o programa SHELL.BIN.
 */
void KeStartShell(){
    panic("KeStartShell:");    //Cancelada! 	
	while(1){}; 
};


/*
 * KeStartTaskManager:
 *     Executa o programa TASKMAN.BIN.
 */
void KeStartTaskManager(){
    panic("KeStartTaskManager:");    //Cancelada! 	
	while(1){};
};


/*
 * start_task: 
 *     Habilita uma tarefa para ser executada pela primeira vez, alertando 
 * sobre a presen�a dela.
 *
 * @todo: 
 *     Poderia ser um kernel request?!
 *
 */
void start_task(unsigned long id, unsigned long *task_address)
{     	
	
	//@todo: filtrar argumentos.
	
	/*
	 * start_new_task_status:
	 *     � uma FLAG. Indica que existe uma thread querendo ser executada 
	 * pela primeira vez.
	 *
	 * @todo: 
	 *     Isso poderia estar na estrutura 'ProcessorBlock' ?!
	 *
	 */
	 
	start_new_task_status = 1; 
    
    //Id e Address.
	start_new_task_id = (int) id; 
    start_new_task_address = (unsigned long) task_address;

    //S� isso ??!!	
	
//Done.
done:	
	return;
};


/*
 * reload_current_task:
 */
void reload_current_task(){
    return;    //Cancelada!
};


/*
 * switch_to_user_mode:     
 */
void switch_to_user_mode(){ 
    panic("switch_to_user_mode:");    //Cancelada!	
	while(1){};
}; 


/*
 * executa_tarefa:
 *     Executa uma nova thread.
 *     @todo: Mudar o nome. startExecuteThread(...)
 *          
 */
//unsigned long startExecuteThread(int tid, unsigned long *thread_address)

unsigned long executa_tarefa(int id, unsigned long *task_address)
{
    struct thread_d *t;
	
    //
	// OBS: A thread a ser executada, precisa ser a current.   
    //
	
	if(current_thread != id)
	{
        printf("executa_tarefa: current_thread = %d | new task = %d ",current_thread, id);
        refresh_screen();
		while(1){};
		//return 1;
	};	

	
    //
    // Structure.
    //
	
	t = (void *) threadList[id];	
	if( (void *) t == NULL)
	{
	    panic("executa_tarefa error: Structure!\n");
	    while(1){};
	}
    else
    {
        // Status.	
	    if(t->state != READY){
	        panic("executa_tarefa error: State!\n",id);
            while(1){}; 	    
	    }else{
	        t->state = RUNNING;
	    };
	
	    //...
	};	
   
	//
	// Running tasks ~ Incrementa o n�mero de tarefas que est�o rodando.
	//
	
    ProcessorBlock.running_tasks++;   
	
	IncrementDispatcherCount(SELECT_ANY_COUNT);

	//verbose.	
    // printf("executa_tarefa: Executando %d \n",current_thread);
	//refresh_screen();
	
	//pilha
	unsigned long eflags = 0x3200;
	unsigned short cs = 0x1B;            
	unsigned long eip = (unsigned long ) task_address;
			
	//segmento de dados.
	asm(" movw $0x0010, %ax ");	   
	asm(" movw %ax, %ds ");
	asm(" movw %ax, %es ");
	asm(" movw %ax, %fs ");
	asm(" movw %ax, %gs ");
	
    asm(" pushl %0" :: "r" (eflags): "%esp");  
    asm(" pushw %0" :: "r" (cs)    : "%esp");             
    asm(" pushl %0" :: "r" (eip)   : "%esp");  
	
    outb(0x20,0x20); 
	asm("sti");
    asm("iret");
	//Nothing.	
fail:
    panic("executa_tarefa error: Return!");
    while(1){};    
};


void KiSwitchToUserMode()
{
    //
	//@todo: Fazer rotina de interface.
	//
	
	switch_to_user_mode();
	//while(1){};    
};


unsigned long KiExecutaTarefa(int id, unsigned long *task_address){
    return 0;  //Cancelada.
};


int KiInitTask(int id)
{
	//@todo: Algum filtro.
	if(id < 0){
	    return (int) 0;	
	};
	//Nothing.
	return (int) init_task(id);
};



void KiInitTasks(){
    init_tasks();
    return;	
};

//
//fim.
//

