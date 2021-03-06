/*
 * File: stdio.c 
 *
 * Descri��o:
 *     +Rotinas de input/output padr�o.
 *     +Algumas rotinas de console.
 *
 * Ambiente:
 *     Apenas kernel mode.
 *
 *
 * @todo: Buffering and pipes.
 *        Priorizar opera��es com disco.
 * 
 * Hist�rico: 
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */
  
 
#include <kernel.h>


//
// Fun��es importadas.
//

extern void background();  //Assembly.
extern void refresh_screen();


/*
 * fclose:
 *     Close a file.    
 */
int fclose(FILE *stream)
{	
	if( (void*) stream != NULL ){
		stream = NULL;
	};		

	//
	// @todo: Implementar.
	//


done:	
	return (int) 0;
}


/*
 * fopen:
 *     Open a file.
 */
FILE *fopen(const char *filename, const char *mode)
{
	//
	// @todo: precisamos de um retorno e de um buffer.
	//        e alocar memoria para a estrutura do arquivo.?? 
	//
	
	//Obs: Talvez chamar essa rotina.
	//unsigned long fsLoadFile( unsigned char *file_name, unsigned long file_address)
	
	//FILE *f;
	
	//f = (void) malloc 
		
	//carregar_arquivo( (unsigned char *) filename, (unsigned long) address);
	 
	printf("fopen: @todo:\n");
    return NULL;	
};


/*
 * scroll:
 *     Scroll the screen in text mode.
 *     Scroll the screen in graphical mode.
 *     @todo Poderiam ser duas fun��es: ex: gui_scroll(). 
 *     @todo: N�o realizar scroll da interface gr�fica.
 *    
 *     *IMPORTANTE
 *     O Que devemos fazer � reordenar as linhas
 *     nos buffers de linhas
 *     para mensagens de texto em algum terminal. 
 *     @todo: Ele n�o ser� feito dessa forma, termos uma disciplica de linhas
 * num array de linhas que pertence � uma janela.
 */
void scroll(void)
{
    unsigned short i;
    unsigned short j;
	
	//In�cio da tela.
	unsigned short *p1 = (unsigned short *) SCREEN_START;
	
	//In�cio da segunda linha.
    unsigned short *p2 = (unsigned short *) (SCREEN_START + 2 * SCREEN_WIDTH);

	//
	// Se estamos em Modo gr�fico (GUI).
    //
	
	if(VideoBlock.useGui == 1)
	{
		//Modificar o backbuffer. BACKBUFFER_BASE
        
		//mover o backbuffer come�ando da segunda linha.
		//exclui a primeira linha e a segunda linha ser� a nova primeira.
		
		//so que na verdade isso deve ser feito apenas para a 
		//janela onde o texto est� e n�o para o backbuffer inteiro.
		//por enquanto fica assim.
		
		
		//apaga a linha.
		for(i = 0; i < 70; i++){
		    //outbyte(" ");    	
		};
		
		//cada linha tem 800 pixel, cada char tem 8 pixel de largura,
		//a altura do char s�o pixel.
		//cada pixel tem 3 bytes.
		
		//move 74 linhas come�ando da segunda.
		
		//destino, origem(segunda linha), quantidade de bytes.
        //memcpy( (void*) BACKBUFFER_BASE, (const void*) BACKBUFFER_BASE + (g_cursor_right*8*3), (size_t) (g_cursor_right*8*3*8*(g_cursor_left-1)));        
        memcpy( (void*) BACKBUFFER_BASE, (const void*) BACKBUFFER_BASE + (100*8*3), (size_t) (100*8*3*8*(g_cursor_left-1)));        		

		//
        //Limpa a �ltima linha.
        //
		
		g_cursor_x = g_cursor_left;
		g_cursor_y = g_cursor_bottom;
		
		for(i = g_cursor_left; i < g_cursor_right; i++){
		    printf("%c",' ');    	
		};
		//Nothing.
		
		
		//
		// Reposiciona o cursor na �ltima linha.
		//
		
        g_cursor_x = g_cursor_left;
		g_cursor_y = g_cursor_bottom;
		
		
		
		goto done;
	};	
	
	//
	// Se estavermos em modo texto.
    //
	
	if(VideoBlock.useGui == 0)	
	{
	    //24 vezes.
        for(i=0; i < SCREEN_HEIGHT -1; i++)
	    {
	        //80 vezes
            for(j=0; j < SCREEN_WIDTH; j++){
                *p1++ = *p2++;
		    };
			//Nothing.
		};
	
	    //80 vezes.
        for(i=0; i < SCREEN_WIDTH; i++){
		    *p1++ = 0x07*256 + ' '; 
	    };
		//Nothing.
		
		goto done;
    };
	


    //Nothing.	
	
done:
	return;
};


/*
 * kclear:
 *     Limpa a tela em text mode.
 */
int kclear(int color)
{ 
    unsigned int i = 0;
	char *vm = (char *) g_current_vm;    //fis=0x000B8000, virtual=0x800000; 


//Graphic Mode.
gui_mode:
	if(VideoBlock.useGui == 1){
	    backgroundDraw(COLOR_BLACK);       
		return (int) 0; 		
	};
//Text Mode.
text_mode:	
	while(i < (SCREEN_WIDTH*SCREEN_HEIGHT*2)) 
    { 
        vm[i] = 219; 
        i++; 
        
        vm[i] = color; 
        i++; 
    };
	
	//Cursor.
	g_cursor_x = 0;
	g_cursor_y = 0;
//done.
done: 
    return (int) 0; 
}; 


/*
 * kclearClientArea:
 *     Limpa a �rea de cliente em text mode.
 *     Limpa 24 das 25 linhas.
 *     N�o limpa a primeira de cima.
 */
int kclearClientArea(int color)
{
	char *vm = (char *) g_current_vm;    //fis=0x000B8000; 	
    unsigned int i = (SCREEN_WIDTH*2);
      
    while(i < (SCREEN_WIDTH*SCREEN_HEIGHT*2)) 
    { 
        vm[i] = 219; 
        i++; 
        
        vm[i] = color; 
        i++; 
    };
	
	//Cursor.
	g_cursor_x = 0;
	g_cursor_y = 0;
    
done:
    return (int) 0; 
}; 


/*
 * kprintf:
 *     Rotina simples de escrita em kernel mode.
 */
int kprintf( char *message, unsigned int line, int color )
{ 
    char *vm = (char *) g_current_vm;    //fis=0xb8000, vir=0x800000; 
    unsigned int i; 

	i = (line*SCREEN_WIDTH*2); 

    while(*message!=0) 
    { 
        if(*message=='\n')
        { 
            line++; 
            i=(line*SCREEN_WIDTH*2); 
            *message++; 
        }
        else
        { 
            vm[i]=*message; 
            *message++; 
            i++; 
            vm[i]= color; 
            i++; 
        }; 
    };
	
done:	
    return (int) 0; 
}; 



/*
 * Segue aqui a fun��o printf.  
 *
 */



//
//prints
//
static int prints( char **out, const char *string, int width, int pad)
{
    register int pc = 0, padchar = ' ';

    if(width > 0) 
	{
	    register int len = 0;
		register const char *ptr;
		
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	};
	
	if( !(pad & PAD_RIGHT) ) 
	{
		for ( ; width > 0; --width){
		    printchar(out,padchar);
			++pc;
		};
	};
	
	for( ; *string ; ++string){
		printchar(out, *string);
		++pc;
	};
	
	for( ; width > 0; --width){
		printchar(out,padchar);
		++pc;
	};

done:	
	return pc;
};


//
//printi
//
static int printi( char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if(i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	};
	
	

	if(sg && b == 10 && i < 0){
		neg = 1;
		u = -i;
	};

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while(u) 
	{
		t = u % b;
		
		if ( t >= 10 )
		    t += letbase - '0' - 10;
		    *--s = t + '0';
		    u /= b;
	};

	if(neg) 
	{
		if ( width && (pad & PAD_ZERO) ) 
		{
		    printchar(out, '-');
			++pc;
			--width;
		}
		else 
		{
			*--s = '-';
		};
	};

	return pc + prints(out, s, width, pad);
};


//
//print
//
static int print(char **out, int *varg)
{
	register int width, pad;
	register int pc = 0;
	register char *format = (char *)(*varg++);
	char scr[2];

	for(; *format != 0; ++format) 
	{
		if(*format == '%') 
		{
			++format;
			width = pad = 0;
			
			if(*format == '\0') 
			    break;
			
			if(*format == '%') 
			    goto out;
			
			if(*format == '-') 
			{
				++format;
				pad = PAD_RIGHT;
			};
			
			while(*format == '0') 
			{
				++format;
				pad |= PAD_ZERO;
			};
			
			for( ; *format >= '0' && *format <= '9'; ++format) 
			{
				width *= 10;
				width += *format - '0';
			};
			
			if( *format == 's' ) 
			{
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			
			if( *format == 'd' ){
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			if( *format == 'x' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if( *format == 'X' ){
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if( *format == 'u' ){
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			if( *format == 'c' ) 
	        {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else 
		{
		    out:
			    printchar (out, *format);
			    ++pc;
		};
	};
	
	if (out) **out = '\0';
	return pc;
};


/*
 * printf:
 *     The printf function.
 *     Assuming sizeof(void *) == sizeof(int).
 */
int printf(const char *format, ...)
{    
    //sincronisa.  
    // vsync();
    
	register int *varg = (int *)(&format);
	return print(0, varg);
};


/*
 * panic:
 *     Kernel panic function.
 *     @todo: Esse fun��o deve retornar void.
 */
int panic(const char *format, ...)
{           
	register int *varg = (int *)(&format);
	
gui_mode:
    if(VideoBlock.useGui == 1)
	{
	    backgroundDraw(COLOR_BLACK);
		printf("KERNEL PANIC:\n\n");
		print(0, varg);
		refresh_screen();
		while(1){}
	};

text_mode:
    if(VideoBlock.useGui == 0)
	{
	    kclear(0);
	    print(0, varg);
	    while(1){}        //No return.
	};
	
//Done.
done:	
	return (int) 0;
};


/*
 * sprintf:
 *     int sprintf ( char * str, const char * format, ... );
 *     Composes a string with the same text that would be 
 * printed if format was used on printf, 
 * but instead of being printed, the content is stored 
 * as a C string in the buffer pointed by str.
*/
int sprintf(char *str, const char *format, ...)
{
	
   // vsync();
	
    register int *varg = (int *)(&format);
	return print(&str, varg);
};


/*
 * fprintf:
 *
 */
int fprintf(FILE *stream, const char *format, ...)
{
    register int *varg = (int *)(&format);
	

	if( (void*) stream != NULL ){
        return 0;   //@todo Error. O erro deveria retornar 1.		
	};		

	char *str = (char *) stream->_ptr;
	
done:	
	return print(&str, varg);	
};


static void printchar(char **str, int c)
{
	//extern int putchar(int c);
	if(str) 
    {
		**str = c;
		++(*str);
	}
	else (void) putchar(c);
};


/*
 * putchar:
 *     Put a char on the screen. (libC).
 */
int putchar(int ch)
{    
    outbyte(ch);
    return ch;    
};


/*
 * The getchar function is equivalent to getc with stdin as the value of the stream argument.
int getchar(void);
int getchar(void)
{
	return 0;
}
*/


/*
 * outbyte:
 *     Trata o caractere a ser imprimido.
 */
void outbyte(int c)
{     
    //@todo: Colocar no buffer de arquivo.
       
    static char prev = 0;
	
	
//checkChar:
        
	//nothing to do.	
    if( c <  ' '  && c != '\r' && c != '\n' && c != '\t' && c != '\b' )
    {
        return;
    };
                
    //Volta ao inicio da linha.        
    if( c == '\r' ){
        g_cursor_x = g_cursor_left;  //0
        prev = c;
        return;    
    };        
       
    
    //In�cio da pr�xima linha.    
    if( c == '\n' && prev != '\r' ) 
    {
        g_cursor_y++;
        g_cursor_x = g_cursor_left;  //0
        prev = c;
        return;
    };
        
    //Pr�xima linha.
	if( c == '\n' && prev == '\r' ) 
    {
        g_cursor_y++;
        prev = c;
        return; 
    };

    //tab
	//@todo: Criar a vari�vel 'g_tab_size'.
    if( c == '\t' )  
    {
        g_cursor_x += (4); 
        prev = c;
        return;         
    };
        
    //space 
    if( c == ' ' )  
    {
        g_cursor_x++; 
        prev = c;
        return;         
    };
        
    //delete 
    if( c == 8 )  
    {
        g_cursor_x--;         
        prev = c;
        return;         
    };
        
     
    //
    // Filtra as dimens�es da janela onde est� pintando.
    // @todo: Esses limites precisam de vari�vel global.
	//        mas estamos usando printf pra tudo.
	//        cada elemeto ter� que impor seu pr�prio limite.
	//        O processo shell impoe seu limite.
	//        a janela impoe seu limite etc...
	//        Esse aqui � o limite m�ximo de uma linha.
    // poderia ser o limite imposto pela disciplina de linha
    // do kernel. para o m�ximo de input. Pois o input �
    // armazenado em uma linha.	 
	//
	
//checkLimits:	

    //Limites para o n�mero de caracteres numa linha.
    if( g_cursor_x >= (g_cursor_right-1) )  
    {
        g_cursor_x = g_cursor_left;
        g_cursor_y++;  
		
    }else{   
        g_cursor_x++;    // Apenas incrementa a coluna.                       
    };
    
	
	//N�mero m�ximo de linhas. (8 pixels por linha.)
    if(g_cursor_y > g_cursor_bottom)  
    { 
	    scroll();
         
	    // DEBUG( tx, "SCROLL!");
         
        // MessageBox("scroll");
        g_cursor_y = g_cursor_bottom;
    };

    // Imprime os caracteres normais.
	_outbyte(c);
	
	// Atualisa o prev.	
	prev = c;	   

done:
    return;
};


/*
 * _outbyte:
 *     Essa rotina envia o caractere para a tela.
 *     @todo: Um switch percebe o modo de v�deo e como esse
 *            caractere deve ser constru�do.
 *            Criar uma estrutura para par�metros de v�deo.
 * 
 */
void _outbyte(int c)
{
    unsigned long i;
	unsigned long x;
	unsigned long y;
	
	//Base atual da mem�ria de v�deo 0x000B8000;
    //@todo: usar VideoBlock.currentVM.
	char *vm = (char *) g_current_vm;    

	char ch = (char) c;
	char ch_atributo = (char) g_char_attrib;
	
	
	//
	// *** #Importante: Essa rotina n�o sabe nada sobre janela, ela escreve na tela 
	// como um todo: s� est� considerando as dimens�es do 'char'.
	//

    /*
     * Caso estivermos em modo gr�fico.
     */
	if(VideoBlock.useGui == 1)
	{
	    //	vsync();
		
		// @todo: A char deve ir para um buffer de linha,
		// antes de ser pintado na janela apropriada.
		
	    switch(VideoBlock.vesaMode)
		{
			//
			// @todo: Testar gcharWidth, gcharHeight.
			//
			
			//A rotina char built considera as dimens�es da tela como um todo 
			//para construir o 'char' no local definido, porem ela n�o sabe nada 
			//sobre janelas.
			
		    //@todo: Listar aqui os modos VESA.
		    case 1:
			    my_buffer_char_blt( 8*g_cursor_x, 8*g_cursor_y, COLOR_WINDOWTEXT, c);
				  //my_buffer_char_blt( gcharWidth*g_cursor_x, gcharHeight*g_cursor_y, COLOR_WINDOWTEXT, c);
			    break;
		    default:
			    //modo gr�fico vesa 640x480 24bpp, 8 pixel por caractere.
			    my_buffer_char_blt( 8*g_cursor_x, 8*g_cursor_y, COLOR_WINDOWTEXT, c);
			    //my_buffer_char_blt( gcharWidth*g_cursor_x, gcharHeight*g_cursor_y, COLOR_WINDOWTEXT, c);
				break;
		};

     	goto done;	
	};
 

	/*
	 * Caso estivermos em text mode.
	 */
    if(VideoBlock.useGui == 0)
	{
	    //calcula o valor do deslocamento para text mode 80x25.
	    y = (g_cursor_y * 80 *2);
        x = g_cursor_x*2;    
        i = y + x;

        
		//envia o caractere.
		vm[i+0] = ch;           //char
        vm[i+1] = ch_atributo;  //atributo (foreground,background)
	};
	
done:    
    return;     
};


/*
 * printf_main:
 *     Essa fun��o testa a fun��o printf() e seus recursos.
 *     Obs: We can implement this test in user mode.
 * Obs:
 *     If you compile this file with
 *     gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 *
 */
int printf_main(void)
{

/*	
	
	int mi;
	int i = 5;
	unsigned int bs = sizeof(int)*8;

	char *np = 0;	
	char *ptr = "Hello world!";
	char buf[80];
	
	

	mi = (1 << (bs-1)) + 1;
	
//	vsync();
	
	printf("%s\n", ptr);
	printf("printf test\n");
	printf("%s is null pointer\n", np);
	printf("%d = 5\n", i);
	printf("%d = - max int\n", mi);
	printf("char %c = 'a'\n", 'a');
	printf("hex %x = ff\n", 0xff);
	printf("hex %02x = 00\n", 0);
	printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf("%d %s(s)%", 0, "message");
	printf("\n");
	printf("%d %s(s) with %%\n", 0, "message");
	
    sprintf(buf, "justif: \"%-10s\"\n", "left"); 
    printf("%s", buf);
	
    sprintf(buf, "justif: \"%10s\"\n", "right"); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %04d zero padded\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %-4d left justif.\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %4d right justif.\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %04d zero padded\n", -3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %-4d left justif.\n", -3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %4d right justif.\n", -3); 
    printf("%s", buf);
*/

done:
	return (int) 0;
};


/*
 * input:
 *     Coloca os caracteres digitados em um buffer, (string). Para depois 
 * comparar a string com outra string, que � um comando.
 * 
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Essa rotina foi criada por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */
unsigned long input(unsigned long ch)
{   
	char c = (char) ch;  //Converte 'unsigned long' em 'char'.
	
    //Limite.
	if(prompt_pos >= PROMPT_SIZE){ 
	    printf("Buffer cheio!\n");	
	    return (unsigned long) 0; 
	};
 
	//
	// Trata o caractere digitado. 
	//
	
	switch(c)
	{
	    //Enter.	
		//case 0x1C:
		case VK_RETURN:
		    prompt[prompt_pos] = (char )'\0';
            goto input_done;			
		    break;

	    //Backspace.
		case 0x0E:
            if(prompt_pos <= 0)
			{ 
			    prompt_pos = 0;
				prompt[prompt_pos] = (char ) '\0';
				break; 
			};
		    
			//Apaga o anterior (no buffer).
			prompt_pos--;
			prompt[prompt_pos] = (char ) '\0';
			
			//Apaga o atual no Backbuffer.
			printf("%c",' ');
			
			//Apaga o anterior no Backbuffer.
			g_cursor_x--;
			g_cursor_x--;
			//printf("%c",' ');
			
			//Volta o cursor.
			//g_cursor_x--;
			break;
			
		//...	
		
        //Para qualquer caractere que n�o sejam os especiais tratados acima.		
		default:
		    prompt[prompt_pos] = c;
		    prompt_pos++;          //incrementa fila.
			putchar(c);
			//printf("%c",' ');
			//g_cursor_x--;
			break;
	};
	
input_more:	
	return 0;

input_done:	
    return VK_RETURN;	
};


//
// End.
//
