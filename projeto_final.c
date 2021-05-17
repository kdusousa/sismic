// ER8.5 USCI_A0 com SPP-C BT
//
// HC-05: VCC = +3.3 V
// HC-05: GND = GND
// HC-05: RXD = P3.3 (USCI_A0: TXD)
// HC-05: TXD = P3.4 (USCI_A0: RXD)
// 9.600 bauds (ACLK), 8 bits, sem paridade, 1 stop

#include <msp430.h>
#include <string.h>





#define TRUE  1
#define FALSE 0
#define LED_RED_BLINK  P1OUT ^= BIT0;
#define LED_GRE_BLINK  P4OUT ^= BIT7;


void adc_config(void);
void ta0_config(void);
void USCI_A0_config(void);
void io_config(void);

void bt_char(char x);
void bt_string(char * x);
void bt_sensor(int x);

volatile char perigo[]= {'P','E','R','I','G','O','!'};
volatile char ok[]= {'T','U','D','O',' ','O','K'};
volatile char texto[]={'S','E','N','S','O', 'R',':'};
volatile int sensor;
volatile char sensor_char;
volatile char flag; flag2;
volatile int size_total, size;



int main(void)
{
   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   adc_config();
   ta0_config();
   USCI_A0_config();
   io_config();
   flag2 = FALSE;

   __enable_interrupt();

   while (TRUE){
       while (flag == FALSE);
       flag = FALSE;
       LED_RED_BLINK;
       LED_GRE_BLINK;
       bt_string(texto); bt_char(' ');
       bt_sensor(sensor);
       bt_char('\n');
       __delay_cycles(100);
       //while(flag2 == FALSE);
       //flag2 = FALSE;


       if (sensor > 250){
           bt_string(perigo);
      }
       else{
          bt_string(ok);
       }
       bt_char('\n');
       bt_char('\n');
       bt_char('\n');


   }
   return 0;
}
//************* Rotina de Interrupção do ADC ************
#pragma vector = ADC12_VECTOR
__interrupt void adc_int(void){
    sensor=ADC12MEM0;
    flag = TRUE;
}



//************* CONFIGURAÇÃO DO ADC e TA0*****************
void adc_config(void){

    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = ADC12ON;

    ADC12CTL1 = ADC12CONSEQ_2 |         // Canal com repetição
                ADC12SHS_1 |            // dispara por TA0.1
                ADC12CSTARTADD_0 |      // Endereço inicial 0
                ADC12SSEL_3;            // SMCLK

    ADC12CTL2 = ADC12RES_2 | ADC12TCOFF; // 12 BITS

    ADC12MCTL0 =  ADC12SREF_0 | ADC12INCH_1 | ADC12EOS;

    P6SEL |= BIT1;
    ADC12IE |= ADC12IFG0;
    ADC12CTL0 |= ADC12ENC;
}

void ta0_config(void) {
    TA0CTL = TASSEL_1 | MC_1;
    TA0CCTL1 = OUTMOD_6;

    TA0CCR0 = 32767; //1 CONVERSÃO POR SEGUNDO
    TA0CCR1 = TA0CCR0 / 2; // Carga 50%
}


// Configurar USCI_A0 (Bluetooth)
void USCI_A0_config(void){
    UCA0CTL1 = UCSWRST;    //RST=1 para USCI_A0
    UCA0CTL0 = 0;          //sem paridade, 8 bits, 1 stop, modo UART
    UCA0BRW = 3;           //Divisor
    UCA0MCTL = UCBRS_3;    //Modulador = 3 e UCOS=0
    P3SEL |= BIT4 | BIT3;  //Disponibilizar pinos
    UCA0CTL1 = UCSSEL_1;   //RST=0, ACLK
}

void io_config(void){

    //Configurar Led Vermelho
    P1DIR |= BIT0;              // Configura pino como saída - P1.0 = 1
    P1OUT &= ~BIT0;             // Led 1 apagado             - P1.0 = 0

    //Configurar Led Vermelho
    P4DIR |= BIT7;              // Configura pino como saída - P1.0 = 1
    P4OUT |= BIT7;             // Led 2 aceso

    //Configurar SW
    P6DIR &= ~BIT4;             //SW
    P6REN |= BIT4;
    P6OUT |= BIT4;
}

// Envio de caracteres

void bt_char(char x){
   UCA0TXBUF = x;
   while ( (UCA0IFG & UCTXIFG) == 0);
   UCA0IFG &= ~UCTXIFG;
}

void bt_string(char * x){
    int i;
    for (i=0; i<7; i++){
        bt_char(x[i]);
    }
}

void bt_sensor(int x){
    int y;
    if (x < 10){
        sensor_char = x + '0';
        bt_char (sensor_char);
    }

    else if (x >= 10 && x < 100){
        y = x/10;
        bt_sensor(y);
        y = x%10;
        bt_sensor(y);
    }

    else if (x >= 100 && x < 1000){
        y = x/100;
        bt_sensor(y);
        y = x%100;
        bt_sensor(y);
    }

    else if (x >= 1000 && x < 10000){
        y = x/1000;
        bt_sensor(y);
        y = x%1000;
        bt_sensor(y);
    }
    //flag2 = TRUE;

}
