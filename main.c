// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = ON
#pragma config FCMEN = ON
#pragma config LVP = OFF
#pragma config BOR4V = BOR40V
#pragma config WRT = OFF

// CONFIG2
#pragma config BOR4V = BOR40V // Brown-out Reset Selection bit(Brown-out Reset set to 4.0V)
#pragma config WRT = OFF // Flash Program Memory Self Write Enablebits (Write protection off)

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#define _XTAL_FREQ 4000
void main(){
    //Configura entradas y salidas
    TRISA=0xFF;
    TRISB=0x00;
    TRISC=0x00;
    TRISC7=1;
    
    //Configura UART a 9600 baudios
    TXSTAbits.TXEN=1;
    TXSTAbits.BRGH=1;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    BAUDCTLbits.BRG16=0;
    SPBRG=25;
    SPBRGH=0;
    
    //Configura puerto AD
    ANSEL=0;
    ANSELbits.ANS1=1; //CAMBIA CUAL ESTA PRENDIDA
    ADCON1bits.ADFM=1;
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    ADCON0bits.ADON=1;
    ADCON0bits.CHS=1;
    
    char aux;
    
    while (1)
    {
        //Medicion de temperatura
        ADCON0bits.GO=1; //ARRANCA
        while (ADCON0bits.GO==1) //ESPERAR A QUE TERMINE DE MEDIR
        aux= (ADRESH<<8)+ADRESL; //GUARDA EL VALOR MEDIDO EN UNA VARIABLE AUXILIAR
        __delay_ms(2);
        if (RCIF == 1){
            if(RCREG == 'A') TXREG=aux; //ENVIA EL RESULTADO
        }
    }
}
