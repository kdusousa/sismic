// Serial para se comunicar com o PC


#include <msp430.h> 

#define CR  0xD //Carriage Return
#define LF  0xA //Line Feed

void ser_spc(void);
void ser_dec8(char x);
void ser_crlf(void);
void ser_char(char x);
void config_serial(void);

void main (void){
    int letra = 'A';
    char cont=0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    config_serial();
    __delay_cycles(10);
    ser_dec8(cont);

    while (1){
        ser_char (letra++);
        if (letra == 'Z'+1){
            letra='A';
            ser_crlf();
            ser_dec8(cont);
            //ser_spc();
            ser_char(' ');
            cont++;
        }
        __delay_cycles(100000);
    }
    return 0;
}

void config_serial(void){
    UCA1CTL1 =  UCSSEL_2 | UCSWRST;     //UCSI em Reset
    UCA1CTL0 = UCSPB;                   //2 Stops

    //Baudrate 9.600
    UCA1BRW = 6;                //Divisor por 3
    UCA1MCTL=UCBRF_13 | UCBRS_0 | UCOS16;

    // 115200
    //UCA1BRW = 9;                //Divisor por 9
    //UCA1MCTL=UCBRF_0 | UCBRS_1;

    P4DIR |=  BIT4;             //P4.4 saída
    P4DIR &= ~BIT5;             //P4.5 entrada
    P4SEL |= BIT5 | BIT4;       //Tx serial

    PMAPKEYID = 0X02D52;    //Liberar mapeamentp
    P4MAP4 = PM_UCA1TXD;    //TX=Sair por P4.4
    P4MAP5 = PM_UCA1RXD;    //RX=Receber por P4.5

    UCA1CTL1 &= ~ UCSWRST;      //UCSI sai de Reset

}


void ser_dec8(char x){
    char z;

    z=x/100;    //separar centena
    ser_char(0x30+z);

    x=x-100*z;
    z=x/10;     //separar dezena
    ser_char(0x30+z);

    x=x-10*z;
    ser_char(0x30+x);
}

void ser_spc(void){
    UCA1TXBUF = ' ';
    while ( (UCA1IFG & UCTXIFG) == 0);
    UCA1IFG &= ~UCTXIFG;
}

void ser_char(char x){
    UCA1TXBUF = x;
    while ( (UCA1IFG & UCTXIFG) == 0);
    UCA1IFG &= ~UCTXIFG;
}

void ser_crlf(void){
    UCA1TXBUF = CR;
    while ( (UCA1IFG & UCTXIFG) == 0);
    UCA1IFG &= ~UCTXIFG;
    UCA1TXBUF = LF;
    while ( (UCA1IFG & UCTXIFG) == 0);
    UCA1IFG &= ~UCTXIFG;
}
