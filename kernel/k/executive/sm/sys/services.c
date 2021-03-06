/*
 * File: services.c 
 *
 * >>>> Quando essa rotina receber uma solicita��o de servi�o
 * ela envia para a CLASSE correspondente � categoria do servi�o solicitado.
 *
 * Descri��o:
 *     Arquivo principal do m�dulo sys do executive.
 *     Servi�os oferecidos pelo kernel via systemcall.
 *     O Kernel faz ponte entre Clientes em user mode e servidores que est�o 
 * em user mode. O cliente em user mode solicita servi�os oferecidos por 
 * servidores que est�o em user mode e o kernel faz a ponte entre o cliente
 * e o servidor.
 *     O kernel estabelece essa conex�o cliente/servidor atrav�z de mensagens 
 * via interrup��o, a interrup��o � a 200. A interrup��o 200 executa a rotina 
 * services(...) para atender as chamadas dos processos cliente. Para atender 
 * essas chamadas o kernel usa rotinas internas ou chama os servidores que 
 * podem estar tanto em kernel modo quanto em user mode.
 *
 * In this file:
 *   + Services
 *   + Dispatch to procedure
 *   + Change procedure
 *
 * @todo:
 *     Todos os servi�os oferecidos est�o nos 
 *     arquivos principais dos m�dulos hal microkernel e executive
 *     e come�am com 'sys_'.
 *
 *
 * @todo: LOGO AP�S IDENTIFICAR O N�MERO DO SERVI�O SOLICITADO,
 *        CHAMAREMOS AS ROTINAS DE SERVI�O COM BASE EM CLASSES
 *        DE SERVI�OS. (Isso � oferecido em system.c). 
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Aprimoramento geral das rotinas b�sicas.
 *     //...
 */


#include <kernel.h>

//
// Constantes internas.
//

#define SERVICE_NUMBER_MAX 255


//
// Vari�veis internas.
//

//int servicesStatus;
//int servicesError;
//...


//
// Prot�tipos de fun��es internas.
//

void servicesChangeProcedure();


//
//...
//

/*
 * services:
 *     Rotina que atende os pedidos feitos pelos aplicativos em user mode 
 *     via int 200. Ou ainda o kernel pode chamar essa rotina diretamente.
 *     S�o v�rios servi�os.
 *
 *
 * obs: >>> TODOS OS SERVI�OS DESSA ROTINA DEVEM CHAMAR ROTINAS DE CLASSES 'system.x.x'
 *
 * @todo: 
 *    Pode haver algum tipo de autoriza��o para essa rotina.
 *    Ou ainda, autoriza��o por grupo de servi�os. Sendo os servi�os
 *    de i/o os de maior privil�gio.
 *
 *    +Identificar o processo que fez a chamada e i/o e
 *    configurar a vari�vel que identifica esse processo.
 *    'caller_process_id' � uma vari�vem encapsulada em process.c
 *    'set_caller_process_id(int pid)' configura a vari�vel.
 *
 *    Uma chamada como OpenDevice pode liberar o acesso
 *    para um determinado processo e CloseDevice cancela o acesso.  
 * 
 *    @todo: O Nome da fun��o, para pertencer � essa pasta, deveria ser:
 *           servicesSystemServices(....).
 */
void *services( unsigned long number, 
                unsigned long arg2, 
				unsigned long arg3, 
				unsigned long arg4 )
{
	//
	// Decalra��es.
	//
	
    //Args. (strings)
	unsigned char *stringZ  = (unsigned char *) arg2;
    unsigned char *stringZZ = (unsigned char *) arg3;	
	unsigned long *a2 = (unsigned long*) arg2;
	unsigned long *a3 = (unsigned long*) arg3;
	unsigned long *a4 = (unsigned long*) arg4;
		
	//Char and string support.
	char *argChar;
	unsigned char* argString;

	//Retorno.
	void* Ret;
	
	//Function(CreateWindow)
	//arg2 =Type
	//arg3=Status
	//arg4=Name
	
	//Window.
	unsigned long WindowType;      //Tipo de janela.
	unsigned long WindowStatus;    //Status, ativa ou n�o.
	unsigned long WindowRect;
	unsigned long WindowView;      //Min, max.
	char *WindowName;            
	
	//
	// Window stuffs:
	//
	
	int ActiveWindow;          //Current id.
	
	struct window_d *hWnd;
	//struct window_d *cWnd;     //Current window.
	//struct window_d *aWnd;     //Active Window. 	
	struct window_d *focusWnd;   //Janela com foco de entrada.
    struct window_d *NewWindow;  //Ponteiro para a janela criada pelo servi�o.
	
	
	//void* kHWND;
	void* kMSG;
	//void* kLONG1;
	//void* kLONG2;
	//...
	
	//
	// Setup.
	//
	
	//Window.
	hWnd = (void*) arg2;

	//
	// @todo: 
	//     Antes de utililizar as dimens�es da �rea de trabalho
	//     vamos atribuir valores negligenciados para as dimens�es.
	//
	
	//Inicializando dimens�es.
	//Obs: Emulando telas de celulares.
	// 320x480.
	// Terminal abaixo dos di�logos.
	unsigned long WindowX = (6*(800/20));  //100;   >          
	unsigned long WindowWidth  = 320;               
	
    unsigned long WindowY = (4*(600/20)); //100;   V                
    unsigned long WindowHeight = 480;  
	
	unsigned long WindowColor = COLOR_WINDOW;  
	unsigned long WindowClientAreaColor = COLOR_WINDOW;  

    struct rect_d *r;	
	
	//
	// Se a �rea de trabalho for v�lida, usaremos
	// suas dimens�es, caso n�o seja, temos um problem.
	//
	
	if(gui->main == NULL){
		printf("systemServices: main");
		refresh_screen();
		while(1){}
	};		
	
	//Apenas posicionamento.
	//if(gui->main != NULL)
	//{
        //WindowX = (unsigned long) (gui->main->width / 3);             
        //WindowY = (unsigned long) (gui->main->height / 4);                   
	    //WindowWidth  = (unsigned long) (gui->main->width / 3);             
        //WindowHeight = (unsigned long) (gui->main->height / 3);       
	//};
	
	//
	// Limits. (Limites do n�mero do servi�o).
	//
	
	if( number < 0 || number > SERVICE_NUMBER_MAX ){
	    return NULL;	
	};

	//Number.
	switch(number)
	{
	    //0 - Null, O processo pode ser malicioso.
	    case SYS_NULL: systemRam(0,0,0,0,0); break; 	   
		
		//Disco: 1,2,3,4
		
		//1 (i/o) Essa rotina pode ser usada por um driver em user mode.
		case SYS_READ_LBA: systemDevicesUnblocked(36,arg2,arg3,0,0); break;
			
		//2 (i/o) Essa rotina pode ser usada por um driver em user mode.
		case SYS_WRITE_LBA: systemDevicesUnblocked(35,arg2,arg3,0,0); break;

		//3 fopen (i/o)
		case SYS_READ_FILE:
		    taskswitch_lock();
	        scheduler_lock();	
            Ret = (void *) fsLoadFile( (unsigned char *) a2, (unsigned long) arg3);    //name , address.  
		    //fopen( const char *filename, const char *mode );
		    //??retorno ??? 1 = fail ; 0=ok.
			scheduler_unlock();
	        taskswitch_unlock();
			return (void*) Ret;
			break;

		//4 (i/o)
		case SYS_WRITE_FILE:
		    taskswitch_lock();
	        scheduler_lock();	
		    //fsSaveFile((unsigned char *) a2, (unsigned long) arg3, (unsigned long) arg4);
			scheduler_unlock();
	        taskswitch_unlock();
		    break;
			
		//Gr�ficos:5,6,7,8,9,10,11.
		
		//5 Vertical Sync.(N�o sei se � necess�rio isso via interrup��o.); 
        case SYS_VSYNC:
		    sys_vsync();       		
			break;
			
		//6
        //Coloca um pixel no backbuffer.
        //Isso pode ser usado por um driver. 		
        case SYS_BUFFER_PUTPIXEL:
            my_buffer_put_pixel( (unsigned long) a2, 
			                     (unsigned long) a3, 
								 (unsigned long) a4, 
								  0 );    //Put pixel. 		
			break;

		//7
		//desenha um char no backbuffer.
		//Obs: Esse funciona, n�o deletar. :)
		// (x,y,color,char)
		// (left+x, top+y,color,char)
		//devemos usar o left e o top da janela com foco d3e entrada.
        //
		case SYS_BUFFER_DRAWCHAR: 			
			focusWnd = (void*) WindowWithFocus; //windowList[window_with_focus];
			if( (void*) focusWnd == NULL ){
			    break;	
			};
			
			my_buffer_char_blt( (unsigned long) (arg2 + focusWnd->left), 
			                    (unsigned long) (arg3 + focusWnd->top), 
								COLOR_BLACK, 
								(unsigned long) arg4);
			break;

		//8
        case SYS_BUFFER_DRAWLINE:
            //@todo: BugBug, aqui precisamos de 4 par�metros.
			my_buffer_horizontal_line( (unsigned long) a2, 
			                           (unsigned long) a3, 
									   (unsigned long) a4, 
									   COLOR_WHITE); 		
			break;

		//9
        case SYS_BUFFER_DRAWRECT:
		    //@todo: BugBug, aqui precisamos de 5 par�metros.
            drawDataRectangle( 0, 
			                  (unsigned long) a2, 
			                  (unsigned long) a3, 
							  (unsigned long) a4, 
							   COLOR_WHITE );    		
			break;

		//10 Create window.
		// O argumento mais importante � o tipo.
        case SYS_BUFFER_CREATEWINDOW: 
		    //if( (void *) arg3 == NULL ){ return NULL; } //rect_d
			
            //printf("service 10 create window\n");		
			WindowType = arg2;           //arg2 Type. 
			//r = (void *) arg3;         //arg3 (Ponteiro para a estrutura rect_d)
            WindowStatus = arg3;         //status
			WindowName = (char *) a4;    //arg4 Window name.
			WindowView = VIEW_MAXIMIZED; 
			goto do_create_window;
            break;
			
			
		//11, Coloca o conte�do do backbuffer no LFB.
        case SYS_REFRESHSCREEN: 
		    systemRam(3,0,0,0,0); 
			break;			
			
        //rede: 12,13,14,15			
		//i/o:  16,17,18,19	
        //Outros: 20 at� o fim.		

		//34	
        case SYS_VIDEO_SETCURSOR: 
		    systemRam(86,arg2,arg3,0,0); 
			break;              

		//35 - Configura o procedimento da tarefa atual.
        case SYS_SETPROCEDURE:  
            g_next_proc = (unsigned long) arg2;
            //return NULL;			
            break;
			
		//36
        //O teclado envia essa mensagem para o procedimento ativo.
        case SYS_KSENDMESSAGE: 
            systemRam(2,(unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4,0); 
            break;    
      
	
		//37 - Chama o procedimento procedimento default. @todo return.
		case SYS_CALLSYSTEMPROCEDURE: 
            system_procedure(NULL,arg2,arg3,arg4);	
            break;    
        
	
        //42 Load bitmap 16x16.
		case SYS_LOADBMP16X16 :       
            load_bitmap_16x16( arg2, arg3, arg4, 0);
            return NULL;			
            break;


        //45 Message Box. 
        case SYS_MESSAGEBOX:
            //Ok Funcionou assim.		
            MessageBox(gui->screen, 1, (char *) a3, (char *) a4 );        
            return NULL;
			break;
		
        //47, Create Window 0.		
        case SYS_BUFFER_CREATEWINDOWx:
			WindowType = 0;  //?? 
			//r = (void*) arg3; 
			WindowStatus = arg3;   //status
			WindowView = 1; 
            WindowName = (char*) a4;
			goto do_create_window;
            break;

		//57.	
		case SYS_REGISTERWINDOW: systemRam(41,(unsigned long) hWnd,0,0,0); break;
		//58.	
		case SYS_CLOSEWINDOW: systemRam(53,(unsigned long) hWnd,0,0,0); break;
			
        //60
		case SYS_SETACTIVEWINDOW:			
			set_active_window(hWnd);
			break;

        //61
		case SYS_GETACTIVEWINDOW:
            return (void*) get_active_window();    //Id. (int).		
			break;

        //62
		case SYS_SETFOCUS: systemRam(71,(unsigned long) hWnd,0,0,0); break;
        //63
		case SYS_GETFOCUS: return (void*) systemRam(72,0,0,0,0); break;
        //64
		case SYS_KILLFOCUS: systemRam(73,(unsigned long) hWnd,0,0,0); break;

		//65-reservado pra input de usu�rio.	
		//case 65:
        //    systemDevicesUnblocked( (int)1, (unsigned long) arg1, (unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4);  		
		//	break;

		//66 - reservado pra input de usu�rio.
		//case 66:
        //    systemDevicesUnblocked( (int)1, (unsigned long) arg1, (unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4);  		
		//	break;

		//67- reservado pra input de usu�rio.	
		//case 67:
        //    systemDevicesUnblocked( (int)1, (unsigned long) arg1, (unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4);  		
		//	break;

		//68- reservado pra input de mouse.	
		//case 68:
        //    systemDevicesUnblocked( (int)1, (unsigned long) arg1, (unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4);  		
		//	break;

		//	
		//69 - Driver de teclado enviando mensagem de digita��o.	
		//     N�o somente um driver, mas qualquer processo pode enviar
		// uma mensagem pra janela com o foco de entrada e chamar o procedimento.
		// >>>> Enviando para a classe certa.
        // arg2=msg, arg3=ch, arg4=ch 0		
		//
		case 69:
			systemDevicesUnblocked( (int) 3, (unsigned long) arg2, (unsigned long) arg3, (unsigned long) arg4, 0);  		
			break;
			
		//70 - Exit. Torna zombie a thread atual.			 
		case SYS_EXIT:
            //Tornando zombie a thread atual se o argumento for 0.
			//Na verdade tem muitos parametros de prote��o pra levar em conta.
			if(arg2 == 0){ sys_exit_thread(current_thread); break; }; 	 //Thread		
            //if(arg2 == 1){ sys_exit_process(arg3,arg4); break; };  //Process.
			//...
			//return NULL;
			break;
		
        //71 		
		case SYS_FORK: return (void*) systemIoCpu(4,0,0,0,0); break;	

		//72 - Create thread.	
        case SYS_CREATETHREAD:			
            //systemIoCpu( 3, 0, 0, 0, 0);
			return (void*) create_thread( NULL, NULL, NULL, arg2, arg3, 0, (char *) a4);
			break; 

		//73 - Create process.	
        case SYS_CREATEPROCESS:
		    //systemIoCpu( 2, 0, 0, 0, 0);
            return (void *) create_process( NULL, NULL, NULL, arg2, arg3, 0, (char *) a4); 		
            break;
			
		//80 Show current process info.
		case SYS_CURRENTPROCESSINFO:
		    //@todo: Mostrar em uma janela pr�pria.
		    show_process_information();
		    break;
			
		//81
		case SYS_GETPPID: 
		    return (void*) sys_getppid();
			break;
		
		//82
		case SYS_SETPPID: 
		    //
			break;
			
		//85
		case SYS_GETPID: 
		    return (void*) sys_getpid();
			break;
		
		//86
		case SYS_SETPID: 
		    //
			break;
		
		//Down. 87
		case SYS_SEMAPHORE_DOWN:
		    return (void*) Down( (struct semaphore_d *) arg2);
		    break;
		//Testa se o processo � v�lido
        //se for v�lido retorna 1234		
		case SYS_88:
            //testando...
            return (void *) processTesting(arg2);			
			break;
			
		//Up. 89	
		case SYS_SEMAPHORE_UP:
		    return (void*) Up( (struct semaphore_d *) arg2);
		    break;
		
		//90 Coletor de threads Zombie. (a tarefa idle pode chamar isso.)		
		case SYS_DEADTHREADCOLLECTOR: systemIoCpu(1,0,0,0,0); break;
			
	    //
        // 99,100,101,102 = Pegar nas filas os par�metros hwnd, msg, long1, long2.
        //		
			
		//**** 99,  Pega 'hwnd' na fila da janela com o foco de entrada.
		case SYS_GETHWINDOW:
		    return (void*) systemDevicesUnblocked(43,arg2,arg2,arg2,arg2);
		    break;
			
		//**** 44, Pega 'msg' na fila da janela com o foco de entrada.
		case SYS_GETKEYBOARDMESSAGE:
		    
			//Pegando a mensagem na fila da janela com o foco de entrada.
		    return (void*) systemDevicesUnblocked(44, 
			                                     (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus);
			break;
			
		//**** 45,  Pega 'long1' na fila da janela com o foco de entrada.	
		case SYS_GETLONG1:
		    return (void*) systemDevicesUnblocked(45, 
			                                     (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus);
			break;
			
		//**** 46,  Pega 'long2' na fila da janela com o foco de entrada.	
		case SYS_GETLONG2:
		    return (void*) systemDevicesUnblocked(46, 
			                                     (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus, 
												 (unsigned long) WindowWithFocus);
			break;	

		//103, SYS_RECEIVEMESSAGE	
        //Um processo consumidor solicita mensagem deixada em seu PCB.
		case SYS_RECEIVEMESSAGE:
            //Argumentos: servi�o, produtor, consumidor, mensagem.		
            //@todo: 
			break;
			
		//104, SYS_SENDMESSAGE
		//Um processo produtor envia uma mensagem para o PCB de outr processo.
		case SYS_SENDMESSAGE:			
		    //Argumentos: servi�o, produtor, consumidor, mensagem.
			//@todo:
			break;

			
		//110 Reboot. (Teclado � legado, � desbloqueado)
	    case SYS_REBOOT: systemDevicesUnblocked(2,0,0,0,0); break;
			
		//...
		
		//
		// 129, Um driver confirmando que foi inicializado.
		// Efetuaremos a rotina de liga��o do driver com o sistema.
		// O sistema linka o driver com o sistema operacional.
	    //@todo: Essa rotina precisa ser aprimorada. Mas a chamada deve ser 
		// essa mesma.
		//
		case SYS_DRIVERINITIALIZED: return (void*) systemLinkDriver(arg2,arg3,arg4); break;
			
		//130
		case SYS_DRAWTEXT:
		    
			//@todo: Podeira pintar na janela atual.
			argString = (unsigned char*) arg4;
		    draw_text( gui->screen, arg2,  arg3, COLOR_TEXT, argString);
			break;
		
		
		case SYS_GETCURSORX:
		    return (void*) get_cursor_x();
		    break;

		case SYS_GETCURSORY:
		    return (void*) get_cursor_y();
		    break;
			
		//144	
		//Pega o ponteiro da client area.	
		case SYS_GETCLIENTAREARECT:	
		    return (void *) getClientAreaRect();	
			break;
		
		//145
        //configura a client area		
		case SYS_SETCLIENTAREARECT:
		    //@todo: O ponteiro para estrutura de ret�ngulo � passado via argumento.
			setClientAreaRect( arg2, arg3, arg4, 0);
            break;


			
			
		//200 - Envia um sinal para um processo.	
		case SYS_SENDSIGNAL:
		    //argumentos (process handle, signal number).
		    signalSend((void*) a2, (int) arg3);
		    break;
		
		//Info. (250 ~ 255).
		
		//250
		//case 250:
		//    break;
		
		//251
		//case 251:
		//    break;

		//252
		//case 252:
		//    break;
		
		//253
		//case 253:
		//    break;
			
			
		//254 - Show PCI info.	
		case SYS_SHOWPCIINFO: systemDevicesBlocked(1,0,0,0,0); break;  
		//255 - Mostra informa��es sobre o kernel.	
		case SYS_SHOWKERNELINFO: systemRam(1,0,0,0,0); break;
			
		//
		// @todo:
		// Need to check the system call ID to ensure it�s valid�
		// If it�s invalid, return ENOSYS �Function not implemented� error
		//
		
		default:	
			printf("Default SystemService={%d}\n",number);
			//return NULL;
			break;
	};
	
	//Debug.
	//printf("SystemService={%d}\n",number);
    
	goto done;	



	
//
// Create window.
//

do_create_window:	
	
	//
	// A primeira coisa a fazer � checar se solicitaram a cria��o
	// de um tipo v�lido de janela.
	// Obs: Ainda n�o conseguimos receber todos os argumentos que essa
	// rotina exige. Uma op��o � que v�rias chamadas sejam feitas,
	// at� que tenhamos todos os argumentos.
	// O argumento tipo � fundamental. Pois com o tipo, podemos oferecer 
	// alguns par�metros padronizados, caracter�sticos do tipo em quest�o.
    //
	
	//
	// Uma solu��o elegante � a cria��o de uma classe de janela.
	// steps:
	// (*1) - Chamada avisando sobre o in�cio de uma sequencia de chamadas
	//      de rotinas de pintura. 
	// *2 - Envio de argumentos que preencher�o a estrutura de classe atual 
	//      e tempor�ria, destinada ao suporte a cria��o de janelas.
	// *3 - Chamada da rotina de cria��o da janela com envio de alguns argumentos.
	//      (Nesse momento a rotina de cria��o da janela deve considerar os 
	//     elementos padr�es caracter�sticos do tipo solicitado, deve considerar 
	//     os argumentos enviados e a estrutura de classe antes preechida pela
	//     chamada anterior.
	// (*4) - Por �ltimo, uma chamada avisa sobre o fim das chamadas de rotinas de 
	//     pintura.
	// A IDEIA � QUE UM PROCESSO N�O SEJA INTERROMPIDO ENQUANTO EST� EFETUANDO
	// SUAS ROTINAS DE PINTURA.
	//
	//
	
	//if(WindowType >= 0 || WindowType <= 5){
	//	
	//}
	
		
	
	//
	// @todo: bugbug.
	// Obs: A parent window � 'gui->screen', talvez poderia ser 'gui->desktop'.
	//      Talvez essa estrutura nem esteja inicializada ainda no ambiente de logon.
	//      O usu�rio s� pode criar janelas na �rea de trabalho do desktop atual.
    //      ...
    //
	
	//
	// @todo: A chamada deve receber, por argumento, um ponteiro para um pacote
	//        de informa��es sobre a janela, contendo sua classe e outros elementos. 
	//
	
	//@todo: deletar
	//r = NULL;
	
	/*
	if( (void*) r == NULL  )
	{
	    NewWindow = (void*) CreateWindow( WindowType, 1, WindowView, WindowName, 
	                                  WindowX, WindowY, WindowWidth, WindowHeight,									  
								      gui->screen, 0, 0, WindowColor);		
	};

	if( (void*) r != NULL  )
	{
		
	    //largura
	    if( r->cx == 0 ){ r->cx = WindowWidth; }
	
	    //altura.
	    if( r->cy == 0 ){ r->cy =  WindowHeight; }
		
	    NewWindow = (void*) CreateWindow( WindowType, 1, WindowView, WindowName, 
	                                  r->x, r->y, r->cx, r->cy,									  
								      gui->screen, 0, 0, WindowColor);		
	};
	
*/
	
	//Cores provis�rias.
	WindowColor = COLOR_WINDOW;
	WindowClientAreaColor = COLOR_TEST_2;
	
	//Criando uma janela, mas desconsiderando a estrutura rect_d passada por argumento.
	//@todo: #bugbug a estrutura rect_d apresenta problema quando passada por argumento.
    NewWindow = (void*) CreateWindow( WindowType, WindowStatus, WindowView, WindowName, 
	                                  WindowX, WindowY, WindowWidth, WindowHeight,									  
								      gui->screen, 0, WindowClientAreaColor, WindowColor);

	if( (void*) NewWindow == NULL )
	{ 
        //?? Mensagem.
	    return NULL; 
	}else{	
        
		//
        // Obs: 
		// Quem solicitou a cria��o da janela pode estar em user mode
        // por�m a estrutura da janela est� em kernel mode. #bugbug
		// Obs:
		// Para preencher as informa��es da estrutura, a aplica��o
		// pode enviar diversas chamadas, Se n�o enviar, ser�o considerados
		// os valores padr�o referentes ao tipo de janela criada.
		// Cada tipo tem suas caracter�sticas e mesmo que o solicitante
		// n�o seja espec�fico nos detalhes ele ter� a janela do tipo que deseja.
        //		
		
        //  
        //@todo: Pode-se refinar os par�metros da janela na estrutura.
		//NewWindow->
		//...
		
		//@todo: N�o registrar, quem criou que registre a janela.
		//RegisterWindow(NewWindow);
	    return (void*) NewWindow; 
	};

//More ...
//xxx:
//yyy:
//zzz:


		
    //
    //    No caso de um aplicativo ter chamado essa rotina, 
	// o retorno ser� para o ISR da int 200, feito em assembly.
    //    No caso do kernel ter chamado essa rotina, apenas retorna.
    //    
    //
	
	
//Done.
done:
    //Debug.
    //printf("Done\n",number);
	//refresh_screen();
    return NULL;	
};


/*
 * servicesChangeProcedure:
 *     Fun��o interna. 
 *     Atende a interrup��o 201, mudando o procedimento de janela atual.
 *     @todo: Passar argumento via registrador. ??
 *     @todo: Outra fun��o j� esta fazendo isso, deletar essa.
 */
void servicesChangeProcedure()
{
	return;
};


//
// Fim.
//

