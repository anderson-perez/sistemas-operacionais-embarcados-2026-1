#include "io.h"

void adc_config(void)
{
    ADCON0bits.CHS      = 0b00000;      // Canal zero (AN0)
    ADCON1bits.PVCFG    = 0b00;         // VDD
    ADCON1bits.NVCFG    = 0b00;         // VSS
    ADCON2bits.ADFM     = 1;            // Justificação do dado (direita)
    ADCON2bits.ACQT     = 0b110;        // Tempo de aquisição do sinal
    ADCON2bits.ADCS     = 0b100;        // Fosc/4
    TRISAbits.RA0       = 1;
    ANSELAbits.ANSA0    = 1;
}

void adc_on(void)
{
    ADCON0bits.ADON = 1;
}

uint16_t adc_read(void)
{
    uint16_t dados_adc;
    
    ADCON0bits.GO = 1;  // Inicia a amostragem do sinal analógico
    
    while (ADCON0bits.GODONE);
    
    dados_adc = ADRES;
    
    return dados_adc;
}

