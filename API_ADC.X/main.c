#include "io.h"

#define LED_ON()    PORTDbits.RD5 = 1
#define LED_OFF()   PORTDbits.RD5 = 0

#define _XTAL_FREQ 4000.0

void config_led(void);
float ler_temperatura(uint16_t dados_adc);

int main()
{
    adc_config();
    adc_on();
    config_led();
    
    while (1) {
        if (ler_temperatura(adc_read()) > 60) {
            LED_ON();
        }
        else {
            LED_OFF();
        }
        __delay_ms(5);
    }
}

void config_led()
{
    TRISDbits.RD5       = 0;
    ANSELDbits.ANSD5    = 0;
}

float ler_temperatura(uint16_t dados_adc)
{
    return (dados_adc * 5.0 / 1023.0) * 100.0;
}
