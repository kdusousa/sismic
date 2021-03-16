#include <msp430.h> 
#include <stdio.h>
#include <stdlib.h>

#define DBC 20000
#define S1  P2IN&BIT1
#define S2  P1IN&BIT1
#define TRUE 1
#define FALSE 0
#define ABERTA 1 //Chave aberta
#define FECHADA 0 //Chave fechada

int mon_s1(void);
int mon_s2(void);
void io_config(void);
void destrava(int z, int *senha);
//void erro(void);
void seq(int x, int y, int z, int i, int *senha);
void certo(int x, int y, int z, int i, int *senha);
void debounce(int a);



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	io_config();
	//char segredo[]={'E','2','D','1','E','3','D','2'}
	int senha[] = {0, 2, 1, 3, 2};
	//int senha[5] = {};
	//trata_senha(segredo, senha);  ver depois
	unsigned int z = 4;
	destrava(z, senha);
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
}

void destrava(int z, int *senha){

    P1OUT |= BIT0;             // Led vermelho aceso             - P1.0 = 1

    unsigned int i = 1;
    unsigned int x = senha[0];
    unsigned int y = senha[i];

    seq(x, y, z, i, senha);


}

/*void erro(void){
    debounce(DBC);
}*/

void seq(int x, int y, int z, int i, int *senha){

    while(1){
        while (y != 0){
                if (x == 0){
                    if (mon_s2() == TRUE)
                        main();
                    else if (mon_s1() == TRUE) {
                        P1OUT &= ~BIT0;
                        y--;
                        seq(x, y, z, i, senha);
                        }
                    debounce(DBC);
                    }
                else if (x == 1){
                    if (mon_s1() == TRUE)
                        main();
                    else if (mon_s2() == TRUE) {
                        P1OUT &= ~BIT0;
                        y--;
                        seq(x, y, z, i, senha);
                        }
                    }
            }
        certo(x, y, z, i, senha);
        while(TRUE);
    }
 }

void certo(int x, int y, int z, int i, int *senha){
    z--;
    while (z != 0){

        x ^= BIT0;
        i++;
        y = senha[i];
        seq(x, y, z, i, senha);
    }
    P4OUT |= BIT7;     //Led verde aceso
    return 0;
    //main();
}

void debounce(int a){
    volatile int x; //volatile evita optimizador do compilador
    for (x=0; x<a; x++); //Apenas gasta
}

// Monitorar S1, retorna TRUE se foi acionada
int mon_s1(void){
    static int ps1=ABERTA; //Guardar passado de S1
    if ( (P2IN&BIT1) == 0){ //Qual estado atual de S1?
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

int mon_s2(void){
    static int ps2=ABERTA; //Guardar passado de S2
    if ( (P1IN&BIT1) == 0){ //Qual estado atual de S2?
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


