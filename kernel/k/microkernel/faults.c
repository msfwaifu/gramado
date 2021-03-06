/*
 * File: faults.c
 *
 *     Rotinas para atender as faults da cpu x86.
 *
 *     Faz parte do Process Manager, parte fundamental do Kernel Base.
 *     As faults de p�ginas s�o usadas para as rotinas de mem�via virtual, 
 * Swap. Ent�o, quando ouver uma falta de p�gina, os m�dulos ou drivers 
 * apropriados devem ser acionados � partir daqui.
 *
 * Obs:
 *     No caso de exce��es de falta de p�gina, por exemplo, o n�cleo tem 
 * informa��o suficiente para tratar completamente o evento e continuar a 
 * execu��o do processo. Por�m, em outros tipos de exce��o, o n�cleo n�o 
 * consegue preceder de maneira inteligente e tem que transferir o 
 * tratamento da exce��o para um processo de tratamento de falha. Esta 
 * transfer�ncia � realizada atrav�s do mecanismo de sinais, no qual o 
 * n�cleo envia ao processo, um sinal correspondente a exce��o ocorrida. 
 * (Wikipedia).
 *
 * @todo: Fazer rotinas para todas as faults.
 * Obs: Talvez o nome mais apropriado seria 'exceptions' en�o 'faults'.
 *
 * History:
 *     2015 - Created by Fred Nora.
 */


#include <kernel.h>


extern unsigned long get_page_fault_adr();


/*
 * faults:
 *     Tratamento de faults.
 */
void faults(unsigned long number)
{
    struct thread_d *t;
	unsigned long page_fault_address;
	
	asm( "cli" );
	
	//kbackground(COLOR_BLACK);
	//set_up_cursor(0,0);
	
	if(VideoBlock.useGui == 0){
		kclear(0);
	};
	
	printf("*FAULTS:\n");

	t = (void *) threadList[current_thread];
	if( (void*) t != NULL )
	{
	    //Salva o conxtexto se a tarefa j� esteve rodando.
	    if( ProcessorBlock.running_tasks >= 1 && t->step > 0 ){
            printf("faults: Saving context\n");
			save_current_context();    
	    }else{	
	        printf("faults: Don't save context\n");
	    };
	}
    else
    {	
	    printf("faults: No context to save\n");
	};
	
	//Info. 
	//(@todo: window station, desktop ...).
	printf("logonStatus={%d}\n",logonStatus);
	printf("guiStatus={%d}\n",guiStatus);
	printf("RunningTasks={%d}\n",ProcessorBlock.running_tasks);	
    printf("t->tid={%d} t->step={%d}\n",current_thread,t->step);			
	printf("FaultNumber={%d}\n",number);                   
    printf("KeInitPhase={%d}\n",KeInitPhase);
	
	//Mostra erro de acordo com o n�mero.
    switch(number)
	{
	    //EXCEPTION
		case 1:
		case 3:
		    printf("EXCEPTION\n");
		    mostra_reg(current_thread);
			break;
		
		//DOUBLE FAULT
	    case 8:
			printf("DOUBLE FAULT\n");
			mostra_slot(current_thread);
		    break;

	    //STACK
	    case 12:
			printf("STACK FAULT\n");
			mostra_reg(current_thread);
		    break;

	    //GP
	    case 13:
		    printf("GP\n");
			mostra_reg(current_thread);
		    break;
		
		//PAGE FAULT
		//Obs: � o contr�rio de page hit.
	    case 14:
		    printf("PAGE FAULT\n");
			//Page Fault Linear Address (PFLA).
			page_fault_address = (unsigned long) get_page_fault_adr();
			printf("Address={%x}\n", (unsigned long) page_fault_address);
			mostra_reg(current_thread);
			mostra_slots();
		    break;
	    
	    default:			
			printf("faults: Default number.\n");
            mostra_reg(current_thread);			
			break;
	};
	
    
	// Done.
    // Final message!	
	// Refresh.	
	// HALT.
done:	
    printf("* System Halted!");    //Bullet.  
	
	if(VideoBlock.useGui == 1){
	    refresh_screen();
	};
halt:
	asm("hlt");   
	while(1){};                      
};


/*
 * KiCpuFaults:
 *     Interface para chamar a rotina de faults.
 */
void KiCpuFaults(unsigned long fault_number){
    faults(fault_number);
	while(1){} 
};


//
// fim.
//

