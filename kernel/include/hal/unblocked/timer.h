/*
 * File: timer.h
 *
 * Descrição:
 *     Header para o módulo timer. irq0.
 *     @todo: strucs ... definições de frequências ...
 *
 * Versão 1.0, 2015.
 */
 
 
//Usado no Linux. 
//#define CT_TO_SECS(x)	((x) / HZ)
//#define CT_TO_USECS(x)	(((x) % HZ) * 1000000/HZ) 
//... 
 
 
//PIT. 
#define TIMER_DRIVER_PATHNAME "/root/drivers/timer.bin"  
 
 
/*
    040-05F = 8253 or 8254 Programmable Interval Timer (PIT, see ~8253~)
	040     = 8253 channel 0, counter divisor
	041     = 8253 channel 1, RAM refresh counter
	042     = 8253 channel 2, Cassette and speaker functions
	043     = 8253 mode control  (see 8253)
	044     = 8254 PS/2 extended timer
	047     = 8254 Channel 3 control byte
*/	
 
/*
 * Como o linux define um Quantum (time slice) para tasks RR.
 */
/*
 * default timeslice is 100 msecs (used only for SCHED_RR tasks).
 * Timeslices get refilled after they expire.
 */
//#define RR_TIMESLICE            (100 * HZ / 1000)



unsigned long time_out;

unsigned long timer_handler_address;    //global _irq0:



typedef struct timer_d timer_t; 
struct timer_d 
{
	//Object.
	object_type_t objectType;
	object_class_t objectClass;
	
	//cal back ;d
	
	
	int dummy;    //c
    //...

    //unsigned long timer_handler_address; 	
	
	unsigned long flag; //f
	unsigned long error; //e
	
	//Navegação.
	struct timer_d * next;
};
//timer_t *Timer;


/*
 * KiTimer:
 * Interface chamada pelo handler da irq0 para um rotina num módulo dentro do 
 * kernel base.
 */
void KiTimer(); 

//Global para inicialização do módulo interno.
int timerInit();
int timerTimer();
void timerInit8253(); 
//...

//Global para acesso ao tempo imediatamente agora.
unsigned long now();

unsigned long get_timeout();
void set_timeout(unsigned long ticks);


//
// End.
//

