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
    
    while (1)
    {
        ADCON0bits.GO=1;
        while (ADCON0bits.GO==1)
        aux2= (ADRESH<<8)+ADRESL;
        __delay_ms(2);
        if (RCIF == 1){
            if(RCREG == 'A')
            {             
                if (aux2 > 1000) {
                    cadena[0] = (aux2/1000)+'0';
                    cadena[1] = ((aux2/100)%10)+'0';
                    cadena[2] = ((aux2/10)%10)+'0';
                    cadena[3] = (aux2%10)+'0';
                    SendSerial(cadena);
                }
                else if (aux2 > 100) {
                    cadena[0] = (aux2/100)+'0';
                    cadena[1] = ((aux2/10)%10)+'0';
                    cadena[2] = (aux2%10)+'0';
                    SendSerial(cadena);
                }
                else if (aux2 > 10) {
                    cadena[0] = (aux2/10)+'0';
                    cadena[1] = (aux2%10)+'0';
                    SendSerial(cadena);
                }
                else{
                    TXREG = (aux2%10)+'0';
                }
            }
        }
    }
}

void SendSerial(unsigned char *data)
{
    unsigned char x=0;
    while(data[x]!='\0'){
        while(PIR1bits.TXIF==0){}   
        TXREG=data[x];
        x++;
    }
}
