#include <xc.h>

void config_ports();
void config_timer0();
void led_1();
void led_2();
void led_3();

void __interrupt() ISR();

typedef void (*ptr_func)(void);

int vez = 0;

ptr_func tarefas[3];

asm("global _led_1, _led_2, _led_3");

int main()
{
    // Instala funções
    tarefas[0] = led_1;
    tarefas[1] = led_2;
    tarefas[2] = led_3;
    
    config_ports();
    config_timer0();
    
    while (1) {
        
    }
    
    return 0;
}

void config_ports()
{
    TRISBbits.RB0       = 0;
    TRISBbits.RB1       = 0;
    TRISBbits.RB2       = 0;
    ANSELBbits.ANSB0    = 0;
    ANSELBbits.ANSB1    = 0;
    ANSELBbits.ANSB2    = 0;    
}

void config_timer0()
{
    INTCONbits.TMR0IE   = 1; // Habilita timer
    INTCONbits.TMR0IF   = 0; // Zera flag do timer
    T0CONbits.T08BIT    = 1; // 8 bits
    T0CONbits.T0CS      = 0; // Instrução interna
    T0CONbits.PSA       = 0; // Ativa preescaler
    T0CONbits.T0PS      = 0b111; // 1:256
    T0CONbits.TMR0ON    = 1; // Ativa timer
    TMR0                = 0;
    INTCONbits.GIE      = 1; // Habilita interrupções
}

void led_1()
{
    while (1) {
        LATBbits.LATB0 = !PORTBbits.RB0;
    }
}

void led_2()
{
    LATBbits.LATB1 = !PORTBbits.RB1;
}

void led_3()
{
    LATBbits.LATB2 = !PORTBbits.RB2;
}

void __interrupt() ISR()
{
    if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;
        TMR0 = 0;
        //tarefas[vez]();
        STKPTR = 0;
        asm("PUSH");
        TOS = (uint24_t)tarefas[vez];
        vez = (vez+1) % 3;
    }
}
