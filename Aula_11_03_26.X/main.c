#include <xc.h>

#define _XTAL_FREQ 4000.0

void config_ports();
void led_azul();
void config_int0();

void __interrupt() isr();

int main()
{
    config_ports();
    config_int0();
    
    while (1) {
//        if (PORTBbits.RB0) {
//            led_azul();
//        }
//        __delay_ms(100);
    }
    
    return 0;
}

void config_ports()
{
    TRISAbits.RA0       = 0; // Output
    ANSELAbits.ANSA0    = 0; // Porta digital    
    TRISBbits.RB0       = 1; // Input
    ANSELBbits.ANSB0    = 0; // Digital
}

void led_azul()
{
    LATAbits.LA0 = !PORTAbits.AN0;
}

void config_int0()
{
    INTCONbits.INT0IE   = 1;  // Habilita int0
    INTCONbits.INT0IF   = 0;  // Flab da int0
    INTCON2bits.INTEDG0 = 1;  // Borda de subida
    INTCONbits.GIE      = 1;  // Interrupções globais
}

void __interrupt() isr()
{
    if (INTCONbits.INT0IF == 1) {
        INTCONbits.INT0IF = 0;
        led_azul();
    }
}