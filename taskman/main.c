/*
 * TASKMAN - Task manager application  for Gramado.
 * (c) Copyright 2015-2017 - Fred Nora
 *
 * File: main.c 
 *
 * Descri��o:
 *     Arquivo principal do aplicativo Task Manager em user mode. 
 *     O nome do utilit�rio � TASKMAN.BIN.
 *     � um aplicativo de 32bit, em user mode, para gerenciar o sistema.
 *     
 * Atribui��es:
 *     Mostrar estat�sticas e status sobre processos e threads, realizar
 * opera��es em processos e threads como criar fechar, etc ...
 *    
 * *IMPORTANTE: ESSE � O GERENCIADOR DE TAREFAS , ELE PODE SOLICITAR
 * MUITAS INFORMA��ES AO KERNEL AJUDANDO O DESENVOLVEDOR A TRABALHAR
 * ESSAS CHAMADAS. � IDEAL PARA TESTES.
 *
 *     Tipo: P3 - Processos em ring3, User Mode.	
 *          
 * Obs:
 *     +O entry point est� em head.s.
 *     +...
 * @todo: + Incluir bibliotecas dispon�veis.
 *        + Dar algum sinal de vida na tela.
 *
 *
 * *** A COR PREDOMINANTE NESSE UTILIT�RIO SER� CINZA ****
 *
 * WIN23
 * #define COLOR_LITBUTTON  0x00E0E0E0
 * #define COLOR_LTGRAY     0x00C0C0C0
 * #define COLOR_GRAY       0x00808080
 *
 * Historico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Aprimoramento geral das rotinas b�sicas.
 *     ...
 */


 
//
// Includes.
//
 
#include "api.h"
#include "stdio.h"
#include "stddef.h"


#define MAGIC (1234)
#define TASKMANAGER_BUFFER_SIZE 512


//
// Vari�veis internas.
//

int taskmanagerStatus;
int taskmanagerError;

//
// Buffer support.
//

char taskmanagerBuffer[TASKMANAGER_BUFFER_SIZE];  
int taskmanagerBufferPos;
//...


//FILE *taskmanOutput;


//
// Prot�tipos de fun��es internas.
//
 
 
//Prot�tipo do procedimento de janela.
int tmProc(int junk, int message, unsigned long long1 , unsigned long long2);

//Prot�tipos de fun��es internas.
void tmUpdateStatus();



void tmSetCursor(unsigned long x, unsigned long y);


/*
tmCreateTaskBar: 
    Barra para mostrar informa��es.
	
Atribui��es:
    +Cria uma barra em baixo na tela com �cones que representam os processos 
da lista de processos, ou que indique atividade de processos.
    +Sondar se o processo existe na lista de processos l� no kernel.
    +Para cada processo encontrado na lista, cria uma janelinha onde ficar� um 
    �cone que representa o processo.
    +...
	
Obs: 
    No futuro, pegaremos essa lista inteira de uma vez s�, atrav�z de �rea 
compartilhada e usaremos a lista aqui em user mode.
    
 */
int tmCreateTaskBar(); 
 
int tmProbeProcessList(int pid);

void tmSleep(unsigned long ms); 

void progress_bar_test();

//Inicializa��es.
int tmInit();



//
//
//


/*
 * sleep:
 *     Apenas uma espera, um delay.
 */
void tmSleep(unsigned long ms) 
{
    unsigned long t = (ms*512);
	
	do{
	    --t;
	}while(t > 0);

done:	
	return;
};


/*
 * tmProbeProcessList:
 *     Sonda para ver se o processo est� presente e � v�lido.
 *     Se retornar 1234 (MAGIC) � um processo v�lido.
 *
 */
int tmProbeProcessList(int pid)
{
	//if(pid < 1){
	//    return (int) -1; //error.	
	//};
	
	return (int) system_call( SYSTEMCALL_88, pid, pid, pid );
};

 
/*
 * tmCreateTaskBar:
 *     ??
 */
int tmCreateTaskBar()
{
	int i;
	int j=0;
	int Magic;
	
	g_cursor_x = 0;
	g_cursor_y = 0;
	
	//@todo: Vari�vel para n�mero m�ximo.
	for(i=0; i<256; i++)
	{
	    Magic = (int) tmProbeProcessList(i);
	    
		if(Magic == MAGIC)
		{
			    //APICreateWindow( 1, 1, 1,"Process",     
                //                 0, j, 20, 20,    
                //                 0, 0, 0 , COLOR_WINDOW );
		        // j = j+22;
				
			
			//@TODO: #BUGBUG: Esta bagun�ando tudo,
			//pintando strings na �rea de cliente de otro processo.
			printf("process magic %d\n",i);	
		};
		//Nothing
	};
	
	
	//nothing
	
Done:
	return (int) 0;
};


/*
 * tmProc:
 *     Procedimento de janela do Task Manager.
 *     O procedimento intercepta algumas mensagens e as mensagens de sistema
 * s�o passadas para o procedimento do sistema na op��o default.
 */
int tmProc( int junk, int message, unsigned long long1, unsigned long long2)
{
    //Filtrar mensagem.
	
	switch(message)
	{
	    case MSG_KEYDOWN:
    		switch(long1)
			{
			    case '1':
					printf("1");
				    break;
					
			    case '9':
				    //taskmanagerStatus = 1234;  
					apiReboot();
				    break;
	
			    case 'a':
				    printf("a");
					break;
			    case KEY_TAB:
				    printf("tab");
				    break;
                default:
				    printf("default key down\n");
					break;
			};
			break;
	    
		case MSG_KEYUP:
		    break;
	
        case MSG_CREATE:
            break; 		
	
	    //...
		
		default:
		    //Falta passar a janela.
			//system_call(SYSTEMCALL_CALL_SYSTEMPROCEDURE, message, long1, long2);			
            break;		
	};

	//Nothing.
	
done:
    return (int) 0;
};


/*
 * tmUpdateStatus:
 *
 */
void tmUpdateStatus()
{
	int CpuStep;
    int ActiveWindow;	
	int wFocus;
	
	//@todo: O n�mero dos servi�os est�o desatualizados.
	
	//pega o valor do step do processador. quantas vezes ele rodou tarefa.
	//CpuStep = system_call(83,0,0,0);
	
	//pega a janela com o foco de entrada.
	//wFocus = system_call(47,0,0,0);
	
	//peaga o numero da janela ativa.
	//ActiveWindow = system_call(45,0,0,0);
	
	
	//tmDrawBar(0x7f);
	tmSetCursor(1,0);
		
	printf("Task Manager: wFocus[%d] | ActiveWindow[%d] | CpuStep[%d]",wFocus
	                                                                  ,ActiveWindow
	                                                                  ,CpuStep );
		
	
	
	//Cursor.
	tmSetCursor(0,1);

	//...
	
done:	
	return;
};


/*
 * tmDrawBar:
 *     Desenhar uma barra em modo texto.
 *    #bugbug N�o � esse o ambiente que estamos. @todo
 */
int tmDrawBar(int color)
{
    unsigned int i = 0;	
	char *vm = (char *) 0x00800000;  //g_current_vm; //phis=0x000B8000; 
	
	//
	// @todo: Usar alguma fun��o de uma biblioteca GUI em user mode.
	//
	
	while(i < ( SCREEN_WIDTH * 2) ) 
    { 
        vm[i] = 219; 
        i++; 
        
        vm[i] = color; 
        i++; 
    };
	
	
	//Cursor.
	tmSetCursor(0,0);
    
	
	
done:	
    return (int) 0; 
};


/*
 * tmSetCursor:
 *     Configurando o cursor. (stdio.h).
 */
void tmSetCursor(unsigned long x, unsigned long y)
{
	//
	// #BUGBUG: Aconteceu uma pagefault depois de incluir essa fun��o. 
	// Vou testar sem ela.
	//
	
    //Atualizamos as vari�veis dentro da estrutura da janela com o foco de entrada.
    //system_call( SYSTEMCALL_SETCURSOR, x, y, 0);	
	

//Atualizando as vari�veis globais usadas somente aqui no shell.
setGlobals:	
    g_cursor_x = (unsigned long) x;
    g_cursor_y = (unsigned long) y;	
	return;
};


//
// strlen:
//     Tamanho de uma string.
// 
size_t tmstrlen(const char *s)
{	
    size_t i = 0;
	
	for(i = 0; s[i] != '\0'; ++i){ 
	; 
	};
	return ( (size_t) i );
};


//
// ================= funny progress bar ===================
//


//
// DoProgress:
//     Credits: Progress bar source code found on 
//     codeproject.com/Tips/537904/Console-simple-progress 
//
void DoProgress( char label[], int step, int total )
{
    //progress width
    const int pwidth = 72;

    //minus label len
    int width = pwidth - tmstrlen( label );
    int pos = ( step * width ) / total ;
   
    int percent = ( step * 100 ) / total;

    //set green text color, only on Windows
    //SetConsoleTextAttribute(  GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_GREEN );
    printf("%s[", label);

    //fill progress bar with =
	int i;
    for( i = 0; i < pos; i++ ){
		printf("%c", '=');
    };
	
    //fill progress bar with spaces
    printf("% *c", width - pos + 1);
    printf("] %3d%%\r", percent);

    //reset text color, only on Windows
    //SetConsoleTextAttribute(  GetStdHandle( STD_OUTPUT_HANDLE ), 0x08 );
	
    return;	
};


void DoSome()
{
    int total = 1000;
    int step = 0;

    while(step<total){
        
		//do some action
        
		tmSleep(5000);
        
		step+=1;
        
		DoProgress("Loading: ",step,total);
    }
    //
done:
    printf("\n");
	return;
};


void progress_bar_test(){
    DoSome();
    return;
};



/*
 * tmInit:
 *     Inicializa��es.
 */
int tmInit()
{
	taskmanagerStatus = 0;
	taskmanagerError = 0;
	//taskmanagerBuffer = '\0'
	taskmanagerBufferPos = 0;
	
	//#DEBUG
	//printf("T");
	//printf("taskman: testing strings \n");
	//tmSetCursor(8,8); 
	//printf("taskman: and cursor. \n");
	//refresh_screen();
	//while(1){}		
	
	//...
	
done:
    return (int) 0;
};


/*
 * appMain:
 *     Fun��o principal do Task Manager.
 * 
 * @todo:
 *     +... 
 */
int appMain(int argc, char *argv[]) 
{
	//int Status;	
	int Flag;
	void *P;
    struct window_d *hWindow;

    //
	// Op��o: Tratar os argumentos recebidos.
	//
	
	
	//#debug
debugStuff:
    // Uma mensagem de sinal de vida.
	MessageBox( 1, "TASKMAN.BIN:", "Initializing ...");
    refresh_screen();
	//while(1){}
	
	
	//
	// Obs:
	//  **** O message box apresenta mensagens corretamente. ****
	// O desafio agora � enviar mensagens para a �rea de cliente 
	// do utilit�rio TaskMan.
	// mensagens de erro ser�o apresentadas via MessageBox.
	//
	
    //Initializing ...	
initializing:	
	tmInit();  
	
	//
	// Criando os processos que compoem o Security Control:
	// ===================================================
	//
	// LOGON  - Logon. 
	// LOGOFF - Logoff
	// OM     - Object Manager. 
	// SS     - System Security.
	//

//	
// Obs: O object manager controlar� algumas permiss�es iniciais
// que habilitam os usu�rios a utilizarem recursos do sistema.
// Quem controla na verdade os objetos � o kernel base, que consede
// acesso aos objetos. Permitindo que grupos, usu�rios, processos e 
// e threads denham acesso aos recursos do sistema. 
//	
creatingSecurityControl:	
	
	//Logon.
	P = (void*) apiCreateProcess( 0x400000, PRIORITY_HIGH,"LOGON");
	if( (void*) P == NULL  ){
	    MessageBox( 1, "TASKMAN.BIN:", "Fail creating process LOGON");		
	    refresh_screen();
		while(1){}
	};

	//Logoff.
	P = (void*) apiCreateProcess( 0x400000, PRIORITY_HIGH,"LOGOFF");
	if( (void*) P == NULL  ){
	    MessageBox( 1, "TASKMAN.BIN:", "Fail creating process LOGOFF");		
	    refresh_screen();
		while(1){}
	};
	
	//Object Manager. 
	//(Pertence ao Security Control).
	P = (void*) apiCreateProcess( 0x400000, PRIORITY_HIGH,"OM");
	if( (void*) P == NULL  ){
	    MessageBox( 1, "TASKMAN.BIN:", "Fail creating process OM");		
	    refresh_screen();
		while(1){}
	};

	//System Security.
	//(Pertence ao Security Control).
	P = (void*) apiCreateProcess( 0x400000, PRIORITY_HIGH,"SS");
    if( (void*) P == NULL ){ 
	    MessageBox( 1, "TASKMAN.BIN:", "Fail creating process SS");		
	    refresh_screen();
		while(1){}
	};
	
	//
	// Obs:
	// Os processos criados qui ainda s�o um conjunto de testes.
	// Estamos criando os processos que compoem o security control,
	// mas n�o estamos associando nenhuma imagens � eles.
	// @todo: Podemos testar a qui a cria��o de algumas threads
	// associadas � esses processos.
	// Obs: Quando criamos um processo, uma thread inicial deve ser criada.
	//
	
    /*
     * @todo: 
	 *     O recurso de criar janelas est� razoavelmente funcionando. 
	 * Mas n�o criaremos janelas ainda. Pois o recurso de janelas est� 
	 * sendo testado e aprimorado no utilit�rio shell. 
	 * Na verdade conv�m testar tudo no shell.
	 * Obs: No futuro, criaremos aqui a janela principal do utilit�rio TaskMan.
	 *
	 */
	 
	 
	// 
    // Create window.
	//
	
	/*
	
	//Tentando enviar strings pra propria janela.
	
	//@todo: Obter o ponteiro para a janela que repressenta a �rea de trabalho.
	//Ou deveria ter uma flag que indicasse, que a janela deve ter como janela mae a �rea
	//de trabalho...
	
	hWindow = (void*) APICreateWindow( WT_OVERLAPPED, 1, 1,"TaskMan Window",     
                                       2, 2, 128, 64,    
                                       0, 0, 0 , COLOR_WINDOW );
	
	if( (void*) hWindow != NULL )
	{
        //Registrar.
	    APIRegisterWindow(hWindow);
	
	    //Configurar como ativa.
	    //APISetActiveWindow(hWindow);
	
	    //Setar foco.
	    //APISetFocus(hWindow);
		
		//Sondando processos. criando �cones.
	    //@todo #bugbug esta pintando na area de cliente de outro processo.
		//tmCreateTaskBar();
	    
		//refresh_screen();
        
        //...		
	};	
	*/

	
    //
	// @todo: 
	//     Fazer alguns testes de chamadas ao sistema.
	// Logo ap�s os teste, pararemos num loop que funcionar� 
	// como pegador de mensagens na fila de mensagens da janela 
	// do processo.
	//
    
	
	//Show info: 
	// As chamadas ao sistema que servem para mostrar informa��es
	// s�o consideradas seguras, perfeitas para essa fase de desenvolvimento
	// do sistem. Apesar de ainda termos problemas com exibi��o das mensagens 
	// na janela correta.
	
	
	//PCI info
    //system_call( SYSTEMCALL_SHOW_PCI_INFORMATION, 0, 0, 0 );
				   
	//kernel info			   
    //system_call( SYSTEMCALL_SHOW_KERNEL_INFORMATION, 0, 0, 0 );	
		
	
	//Testing String
    // ?? do que se trata isso ??
    // Me parece um bom teste.	
	//system_call( SYSTEMCALL_DRAWTEXT, 0,  0, (unsigned long) "/TASKMAN.BIN Testing strings...");
		
		
	//Refresh screen
	//system_call( SYSTEMCALL_REFRESHSCREEN, 0, 0, 0);
	//refresh_screen();
	
	
	
   	//
	// * Hang.
	//
	
getmessageLoop:	
	
	// Debug:
	MessageBox( 1, "TASKMAN:", "Get message loop");	
	refresh_screen();
	//while(1){}
    
	while(1)
	{
        //
        // Test: 		
        // Aproveitando para testar chamadas.		
		// Usando o loop para cauasr um pouco de stress, com muitas chamadas.
		//
		
		//system_call( SYSTEMCALL_NULL, 0, 0, 0 );
        //system_call( SYSTEMCALL_DEAD_THREAD_COLLECTOR, 0, 0, 0 );
		//...
		
		//
		// Sinal de vida.
		// Pintando caracteres no canto superior esquerdo da janela.
		//
		
		//#bugbug ... isso impediu do procedimento de janela do kernel 
		//receber mensagens do teclado. Talvez outro procesimento foi acionado.
		//#bugbug: Cria tantas janelas assim pode nos causa problemas...
		//pois as janelas ainda n�o s�o delatadas...
		//MessageBox( 1, "Message", "Testing TASKMAN application.");
		
		
	    //tmSetCursor(0,0);		
		//printf("\\");
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 		

	    //tmSetCursor(0,0);	
		//printf("|");
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 		

	    //tmSetCursor(0,0);	
		//printf("/");
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 		

	    //tmSetCursor(0,0);	
		//printf("-"); 
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 		

	    //tmSetCursor(0,0);	
		//printf("|"); 
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 		

	    //tmSetCursor(0,0);	
		//printf("-");
		//tmSleep(12345);

	    //tmSetCursor(0,0);		
		//printf(" "); 
		//tmSleep(12345); 

        //progress_bar_test();	

		//
		//taskmanagerStatus
		//
		
		//if(Flag == 1234){
		//	goto done;
		//}
		
		//if(Flag == 1234){}
		//if(Flag == 1234){}
		//if(Flag == 1234){}
		//...
	};	
	
   
    //
    // Obs: 
	// O exit(0) funcionou corretamente no teste.
    // Mas n�o usaremos exit(0) ainda pois temos poucas threads no sistema.
	//
	
done:   
   //exit(0);         //Obs: N�o usar por enquanto. 
   return (int) 0;    //Obs: Retornar com um valor para cr0.s ou head.s.
};



//
// End.
//

