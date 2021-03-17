// 160057701 Carlos Eduardo de Sousa

#include <msp430.h> 

#define DBC 20000
#define S1  P2IN&BIT1
#define S2  P1IN&BIT1
#define TRUE 1
#define FALSE 0
#define ABERTA 1 //Chave aberta
#define FECHADA 0 //Chave fechada
#define LED_RED_ON P1OUT |= BIT0;
#define LED_RED_OFF P1OUT &= ~BIT0
#define LED_GRE_ON  P4OUT |= BIT7
#define LED_GRE_OFF P4OUT &= ~BIT7

int mon_s1(void);
int mon_s2(void);
void io_config(void);
void comando(void);
void debounce(int a);

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	io_config();
	comando();

	return 0;
}

void comando(void){
    int i = 0;
    while (TRUE){
        if (mon_s2() == TRUE){
            debounce(DBC);
            switch (i) {
            case 0:
                break;
            case 1:
                LED_RED_ON;//acende led vermelho
                break;
            case 2:
                LED_RED_OFF;//Apaga led vermelho
                break;
            case 3:
                LED_GRE_ON; //Acende led verde
                break;
            case 4:
                LED_GRE_OFF; //Apaga led verde
                break;
            default:
                break;
            }
            i = 0;
        }
        if (mon_s1() == TRUE){
            debounce(DBC);
            i++;
        }
    }
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
}

void debounce(int a){
    volatile int x; //volatile evita optimizador do compilador
    for (x=0; x<a; x++); //Apenas gasta
}

// Monitorar S1, retorna TRUE se foi acionada
int mon_s1(void){
    static int ps1=ABERTA; //Guardar passado de S1
    if ( (S1) == 0){ //Qual estado atual de S1?
        if (ps1==ABERTA){ //Qual o passado de S1?
            debounce(DBC);
            ps1=FECHADA;
            return TRUE;
        }
    }
    else{
        if (ps1==FECHADA){ //Qual o passado de S1?
            debounce(DBC);
            ps1=ABERTA;
            return FALSE;
        }
    }
    return FALSE;
}


// MONITORA S2
int mon_s2(void){
    static int ps2=ABERTA; //Guardar passado de S2
    if ( (S2) == 0){ //Qual estado atual de S2?
        if (ps2==ABERTA){ //Qual o passado de S2?
            debounce(DBC);
            ps2=FECHADA;
            return TRUE;
        }
    }
    else{
        if (ps2==FECHADA){ //Qual o passado de S2?
            debounce(DBC);
            ps2=ABERTA;
            return FALSE;
        }
    }
    return FALSE;
}



