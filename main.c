//Autor: Brandon Garrido - 19421
//Fecha de creacion: 12/07/2021
//Ultima modificación: -/-/-


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements



//------------------------------------------------------------------------------
//                          Importación de librerías
//------------------------------------------------------------------------------  

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------  

#define _XTAL_FREQ 4000000 //Para delay



//------------------------------------------------------------------------------
//                       Palabras de configuración
//------------------------------------------------------------------------------  

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits 
#pragma config WDTE = OFF       // Watchdog Timer Enable bit 
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit 
#pragma config CP = OFF         // Code Protection bit 
#pragma config CPD = OFF        // Data Code Protection bit 
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = OFF        // Low Voltage Programming Enable bit 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.




//------------------------------------------------------------------------------
//                          Variables
//------------------------------------------------------------------------------

//volatile uint8_t *FILAS[]={0,0,0,0,0,0,0,0};
//volatile uint8_t *COLUMNAS[]={0,0,0,0,0,0,0,0};
int n;
int i;
int x;
int j;
int coso = 0;

uint8_t inicio = 0;
uint8_t end = 0;
uint8_t cFila=1;
uint8_t cColumna=1; 

uint8_t longitud = 1;
uint8_t semillaF;
uint8_t semillaC;

uint8_t easyFlag = 1;
uint8_t normalFlag = 0;
uint8_t hardFlag = 0;

uint8_t upFlag = 0;
uint8_t downFlag = 0;
uint8_t leftFlag = 0;
uint8_t rigthFlag = 0;

uint8_t banderaMulti = 0;
uint8_t disparadorComida = 0;
uint8_t prevFlagDirec = 0;

uint8_t semillaUp=0;
uint8_t semillaDown=0;
uint8_t semillaLeft=0;
uint8_t semillaRigth=0;



//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);
void IOCB_interrupt(void);
void configurarPuertos(void);
void filas(uint8_t fila, uint8_t value);
void columnas(uint8_t columna, uint8_t value);
void TMR0_interrupt();
void reiniciar();

//------------------------------------------------------------------------------
//                          Loop principal
//------------------------------------------------------------------------------


void main(void) {

    setup(); //configuraciones de modulos y timers
    
    
    while(1){ //loop forever
        
        if(!end){
            PORTAbits.RA0 = 0;
        }
                
        if (inicio){
        
            if (!end){
       
                if(PORTAbits.RA1==0 && PORTAbits.RA2==1 && PORTAbits.RA3==1){
                    __delay_ms(1000);
                }else if(PORTAbits.RA1==1 && PORTAbits.RA2==0 && PORTAbits.RA3==1){
                    __delay_ms(600);
                }else if(PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==0){
                    __delay_ms(300);
                }
                

                //colisión
                if(cFila == semillaF && cColumna == semillaC){
                    //generar números aleatorios
                    semillaF = (rand() % 8);
                    semillaC = (rand() % 8);

                    semillaF = semillaF + 1;
                    semillaC = semillaC + 1; 
                    longitud ++;
                    
                }
                
                if(longitud>1){
                        coso = 1;
                    }

                if(upFlag){
                    

                    cFila --;
                    
                    
                    
                    semillaDown = 0;
                    semillaLeft = 0;
                    semillaRigth =0 ;
                    
                 /*   filas(cFila,1);
                    for (x = 1; x < 9; x++) {
                        if(x != cFila){
                            filas(x,0);
                        }
                    }*/

                }else if(downFlag){

                    
                    cFila ++;
                    
                    
                    semillaUp = 0;
                    semillaLeft = 0;
                    semillaRigth =0 ;
                    
                   /* filas(cFila,1);
                    for (x = 1; x < 9; x++) {
                        if(x != cFila){
                            filas(x,0);
                        }
                    }*/

                }else if(leftFlag){

                    
                    cColumna --;
                   
                
                    semillaUp = 0;
                    semillaDown = 0;
                    semillaRigth =0 ;
                    
                   /* columnas(cColumna,0);
                    for (x = 1; x < 9; x++) {
                        if(x != cColumna){
                            columnas(x,1);
                        }
                    }*/

                }else if(rigthFlag){

                    
                    cColumna ++;
                    
                 
                    semillaUp = 0;
                    semillaDown = 0;
                    semillaLeft =0 ;
                    
                    /*columnas(cColumna,0);
                    for (x = 1; x < 9; x++) {
                        if(x != cColumna){
                            columnas(x,1);
                        }
                    }  
                    ¨*/
                }

                if(cColumna == 9 || cFila == 9 || cColumna == 0 || cFila == 0){
                    end = 1;
                }

            }else{

                PORTAbits.RA0 = 1;

            }
        
        }      
       
    }


}


void TMR0_interrupt(){    
       
    if (!end){
        
        if(banderaMulti){//multiplexar comida
            
            for (i = 1; i < 9; i++) {
                if(i != semillaF){
                    filas(i,0);
                }
            }
            
            for (i = 1; i < 9; i++) {
                if(i != semillaC){
                    columnas(i,1);
                }
            } 
            
            filas(semillaF,1);
            columnas(semillaC,0);
            
            banderaMulti = 0;
        }else{//multiplexar snake          
            
            for (x = 1; x < 9; x++) {
                if(x != cFila){
                    filas(x,0);
                }
            }

            for (x = 1; x < 9; x++) {
                if(x != cColumna){
                    columnas(x,1);
                }
            } 

            filas(cFila,1);
            columnas(cColumna,0);

            
            if(longitud==2){
                if(upFlag){                    
                    filas(cFila+1,1);                     
                }else if(downFlag){
                    filas(cFila-1,1);                
                }else if(leftFlag){
                    columnas(cColumna+1,0);                
                }else if(rigthFlag){
                    columnas(cColumna-1,0);                
                }  
            }   

              
           

            banderaMulti = 1;
        }
               

    }
    
    
    
    TMR0 = 247; // valor de n para t=3ms
    INTCONbits.T0IF = 0;
    
    return;
}




void __interrupt() isr(void){

    if (INTCONbits.T0IF){//se verifica que bandera de interrupcion se levanto
        TMR0_interrupt();
    }
    
    if(INTCONbits.RBIF){
        IOCB_interrupt();
    }

}

void IOCB_interrupt(){ // se verifica el push presionado e incrementa o decrem..

    if(upFlag){
        prevFlagDirec = 1;
    }else if(downFlag){
        prevFlagDirec = 2;
    }else if(leftFlag){
        prevFlagDirec = 3;
    }else if(rigthFlag){
        prevFlagDirec = 4;
    }
    
    if (PORTBbits.RB0 == 0){
        
        upFlag = 1;
        downFlag = 0;
        leftFlag = 0;
        rigthFlag = 0;

    }else if (PORTBbits.RB1 == 0){
        
        upFlag = 0;
        downFlag = 1;
        leftFlag = 0;
        rigthFlag = 0;
        inicio = 1;
        if(end){
            reiniciar();
        }
        

    }else if (PORTBbits.RB2 == 0){
            
        upFlag = 0;
        downFlag = 0;
        leftFlag = 1;
        rigthFlag = 0;

    }else if (PORTBbits.RB3 == 0){
        
        upFlag = 0;
        downFlag = 0;
        leftFlag = 0;
        rigthFlag = 1;
        inicio = 1;
        
    }

       /* //CaraFeliz();
        int n;
        int i;
        for (n = 0; n < 8; n++) {
            PORTC = 0xff - (int)pow(2, n);
            for (i = 0; i < 8; i++) {
            PORTD = pow(2, i);            
            __delay_ms(500);

            }

        }
       */
    INTCONbits.RBIF = 0;
    
    return;
} 


void reiniciar(){
    upFlag = 0;
    downFlag = 0;
    leftFlag = 0;
    rigthFlag = 0;

    coso = 0;
    
    banderaMulti = 0;

    inicio = 0;
    end = 0;
    cFila=1;
    cColumna=1; 

    longitud = 1;
    //generar números aleatorios
    semillaF = (rand() % 8);
    semillaC = (rand() % 8);

    semillaF = semillaF + 1;
    semillaC = semillaC + 1; 

}

void filas(uint8_t fila, uint8_t value){
    switch (fila){
        case 0x01:
            PORTDbits.RD5 = value;
            break;
        case 0x02:
            PORTCbits.RC4 = value;
            break;
        case 0x03:
            PORTAbits.RA7 = value;
            break;
        case 0x04:
            PORTCbits.RC6 = value;
            break;
        case 0x05:
            PORTDbits.RD1 = value;//1
            break;
        case 0x06:
            PORTAbits.RA6 = value;
            break;
        case 0x07:
            PORTDbits.RD0 = value;//0
            break;
        case 0x08:
            PORTCbits.RC1 = value;
            break;
        default:
            break;
    }
}

void columnas(uint8_t columna, uint8_t value){
    switch (columna){
        case 0x01:
            PORTCbits.RC5 = value;
            break;
        case 0x02:
            PORTCbits.RC3 = value;
            break;
        case 0x03:
            PORTCbits.RC2 = value;
            break;
        case 0x04:
            PORTDbits.RD4 = value;
            break;
        case 0x05:
            PORTCbits.RC0 = value;
            break;
        case 0x06:
            PORTCbits.RC7 = value;
            break;
        case 0x07:
            PORTDbits.RD3 = value;
            break;
        case 0x08:
            PORTDbits.RD2 = value;
            break;
        default:
            break;
    }
}
/*
    *FILAS [0] = &PORTDbits.RD5;
    *FILAS [1] = &PORTCbits.RC4;
    *FILAS [2] = &PORTAbits.RA7;
    *FILAS [3] = &PORTCbits.RC6;
    *FILAS [4] = &PORTDbits.RD1;
    *FILAS [5] = &PORTAbits.RA6;
    *FILAS [6] = &PORTDbits.RD0;
    *FILAS [7] = &PORTCbits.RC1;
    
    *COLUMNAS [0] = &PORTCbits.RC5;
    *COLUMNAS [1] = &PORTCbits.RC3;
    *COLUMNAS [2] = &PORTCbits.RC2;
    *COLUMNAS [3] = &PORTDbits.RD4;
    *COLUMNAS [4] = &PORTCbits.RC0;
    *COLUMNAS [5] = &PORTCbits.RC7;
    *COLUMNAS [6] = &PORTDbits.RD3;
    *COLUMNAS [7] = &PORTDbits.RD2;
 */

void setup(){
       
    //configuración de reloj interno
    OSCCONbits.IRCF = 0x06; // 110 - 4MHz
    OSCCONbits.SCS = 1; // Habilitar reloj interno
    //configuración interrupciones
    
   
    //configuración interrupciones
    INTCONbits.GIE = 1; // Se habilitan las interrupciones globales
    
    INTCONbits.RBIE = 1; // habilitar banderas de interrupción puertos B
       
    INTCONbits.T0IE = 1; // Se habilitan la interrupción del TMR0
    INTCONbits.T0IF = 0; // Se limpia la bandera
    
     //configuración timer0
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; // PS 111 = 256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    
    TMR0 = 247; // valor de n para t=3ms
    INTCONbits.T0IF = 0;
    
    
    //configuración I/O
    ANSELH = 0;
    ANSEL = 0;

    TRISA = 0B00001110;
    TRISB = 0x0F; // habilitar pines RB0, RB1, RB2 como inputs
    TRISC = 0x00; //
    TRISD = 0x00; //

    OPTION_REGbits.nRBPU =  0 ; // se habilita el pull up interno en PORTB
    WPUB = 0x0F;  // se habilita los pull ups para los pines RB0 y RB1 y RB2
    
    PORTA = 0x00;
    PORTB = 0x0F; // se limpian las salidas de los puertos y valores iniciales
    PORTC= 0x00;
    PORTD= 0x00;

    INTCONbits.RBIF = 0;  
    //configuración iocb
    IOCB = 0x0F; // setear interrupciones en los pines RB0 y RB1 
    

    //configuración inicial 
    
    for (x = 1; x < 9; x++) {
        filas(x,0);
    }
    
    
    for (x = 1; x < 9; x++) {
        columnas(x,1);
    } 
    
    srand(8);
    
    //generar números aleatorios
    semillaF = (rand() % 8);
    semillaC = (rand() % 8);

    semillaF = semillaF + 1;
    semillaC = semillaC + 1; 
    
    
}



