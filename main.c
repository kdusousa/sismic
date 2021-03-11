#include <msp430.h> 
#include "clock.h"
#include "pmm.h"

void io_config(void);

void main()
{
    // Stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	// Increase core voltage so CPU can run at 16Mhz
	// Step 1 allows clocks up to 12MHz, step 2 allows rising MCLK to 16MHz
	pmmVCore(1);                             
	pmmVCore(2);

	// Configure clock
    // This should make MCLK @16MHz, SMCLK @1MHz and ACLK @32768Hz
	clockInit();

	//ex_14();
	//ex_15();
	//ex_16();
	//ex_17();



	return 0;
}

//Configuração inicial GPIO
void io_config(void){

    //Configurar Led Vermelho
    P1DIR |= BIT0;              // Configura pino como saída - P1.0 = 1
    P1OUT &= ~BIT0;             // Led 1 apagado             - P1.0 = 0

    //Configura Led Verde
    P4DIR |= BIT7;              // Led 2 = saída
    P4OUT &= ~BIT7;             // Led 2 apagado

    //Configura chave S1
    P2DIR &= ~BIT1;              // Configura chave como entrada - P2.1 = 0
    P2REN |= BIT1;               // Habilita resistor
    P2OUT |= BIT1;               // Habilita pullup

    //Configura chave S2
    P1DIR &= ~BIT1;              // Configura chave como entrada - P1.1 = 0
    P1REN |= BIT1;               // Habilita resistor
    P1OUT |= BIT1;               // Habilita pullup


void ex_14(void){
    // Acende Led 1
    io_config();
    P1OUT |= BIT0;             // Led 1 aceso

    while (1){
        P1OUT ^= BIT0;
        P4OUT ^= BIT7;
        for (i = 0, i < 1000, i++){
            }
        }
}

void ex_15(void){
    io_config();

    while (1){
        if ((P2IN&BIT1) == 0)
            P1OUT |= BIT0; //Led aceso
        else
            PIOUT &= ~BIT0; //Led apagado
        if ((P1IN&BIT1) == 0)
            P4OUT |= BIT7; //Led aceso
        else
            P4OUT &= ~BIT7; //Led apagado
        }
}
