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
#pragma config BOR4V = BOR40V
#pragma config WRT = OFF

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

#define _XTAL_FREQ 4000

void SendSerial(unsigned char *data);

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
    ANSELbits.ANS1=1;
    ADCON1bits.ADFM=1;
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    ADCON0bits.ADON=1;
    ADCON0bits.CHS=1;
   
    unsigned int aux2;
    char cadena[5];
    
    while(1){
        //Se mantiene en un bucle infinito hasta que recibe un caracter en la UART
        if(RCIF == 1){
            c=RCREG;
            //Verifica si ese caracter es la letra 'A' (mayuscula) o la 'a' (minuscula)
            if(c=='A' || c=='a'){
                //Realiza la conversion AD y espera que finalize
                ADCON0bits.GO = 1;
                while(ADCON0bits.GO == 1)
                //Reconstrucción del valor digital en un único registro de 16 bits
                D=(ADRESH<<8)|ADRESL;
                //Uso de la función printf para convertir el valor en una cadena de caracteres, la función putch enviará los mismos a la UART
                //Agrega caracteres \n y \r al final para que el termina pase a la linea de abajo
                printf("Valor: %d\n\r", D);
            }
        }
    }
}

void putch(char c){
    //Espera que el registro TXREG de la UART esté vacío
    while(PIR1bits.TXIF == 0)
    //Envía caracter hacia el terminal serie
    TXREG = c;
}
