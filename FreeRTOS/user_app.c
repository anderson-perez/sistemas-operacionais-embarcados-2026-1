#include <p24FJ128GA010.h>

#include "user_app.h"
#include "FreeRTOS.h"
#include "task.h"
#include <xc.h>
#include "semphr.h"

#if EXAMPLE_1 == 1

SemaphoreHandle_t s;


void config_user_app()
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    s = xSemaphoreCreateBinary();
}

void task_1()
{
    while (1) {
        LATDbits.LATD0 = ~PORTDbits.RD0;
        xSemaphoreTake(s, portMAX_DELAY);    
    }
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD1;
        //vTaskDelay(10);
        xSemaphoreGive(s);
    }
}

#elif FILOSOFOS == 1

#define QTD 5

// vetor de garfos
SemaphoreHandle_t garfos[QTD];

void config_user_app()
{
    for (int i = 0; i < QTD; i++) {
        garfos[i] = xSemaphoreCreateBinary();   // Cria semáforo
        xSemaphoreGive(garfos[i]);              // Seta em 1
    }
    
    // Configuração de portas dos leds que irão representar os filósofos
    
}

void f_0()
{
    while (1) {
        pensar();
        comer(0);
    }
}

void f_1()
{
    
}

void f_2()
{
    
}

void f_3()
{
    
}

void f_4()
{
    
}

void pensar()
{
    vTaskDelay(50);
}

void comer(int f_id)
{
    // Garfo da esquerda
    xSemaphoreTake(garfos[f_id], portMAX_DELAY);
    // Garfo da direita
    xSemaphoreTake(garfos[(f_id+1) % QTD], portMAX_DELAY);    
}

#elif EXAMPLE_2 == 1

SemaphoreHandle_t s_contador;

void config_user_app()
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    s_contador = xSemaphoreCreateCounting(5, 0);
}

void task_1()
{
    for (;;) {
        xSemaphoreTake(s_contador, portMAX_DELAY);
        LATDbits.LATD0 = ~PORTDbits.RD0;
        vTaskDelay(10);
    }
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD1;
    }
}

#elif EXAMPLE_3 == 1

SemaphoreHandle_t mutex;

void config_user_app()
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    mutex = xSemaphoreCreateMutex();
}

void task_1()
{
    for (;;) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        LATDbits.LATD0 = ~PORTDbits.RD0;
        vTaskDelay(5);
        xSemaphoreGive(mutex);
    }
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD0;
        vTaskDelay(5);        
    }
}

#elif EXAMPLE_4 == 1

QueueHandle_t fila;


void config_user_app()
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    
    // Barra de leds
    TRISGbits.TRISG0 = 0;
    TRISGbits.TRISG1 = 0;
    TRISGbits.TRISG2 = 0;
    TRISGbits.TRISG3 = 0;
    TRISGbits.TRISG6 = 0;
    
    fila = xQueueCreate(5, sizeof(int));
}

void task_1()
{
    int dados[] = {1, 2, 3, 4, 5}, index = 0;
    
    for (;;) {
        LATDbits.LATD0 = ~PORTDbits.RD0;
        xQueueSend(fila, &dados[index], portMAX_DELAY);
        vTaskDelay(5);
        index = (index + 1) % 5;
    }
}

void task_2()
{
    int dado;
    
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD0;
        xQueueReceive(fila, &dado, portMAX_DELAY);
        
        switch (dado) {
            
            case 1: LATGbits.LATG0 = ~PORTGbits.RG0;
                    break;
                    
            case 2: LATGbits.LATG1 = ~PORTGbits.RG1;
                    break;
                    
            case 3: LATGbits.LATG2 = ~PORTGbits.RG2;
                    break;
                    
            case 4: LATGbits.LATG3 = ~PORTGbits.RG3;
                    break;
                    
            case 5: LATGbits.LATG6 = ~PORTGbits.RG6;
                    break;
        }    
        
        vTaskDelay(5);        
    }
}

#elif ALARME == 1

QueueHandle_t mensagem;
SemaphoreHandle_t s;

void config_user_app()
{
    TRISDbits.TRISD0    = 1;
    TRISFbits.TRISF3    = 0;
    TRISGbits.TRISG12   = 0;
    
    mensagem = xQueueCreate(1, sizeof(char));
    s = xSemaphoreCreateBinary();
}

void task_sensor()
{
    char dado;
    
    for (;;) {
        if (PORTDbits.RD0 == 1) {
            dado = 'L';
            xQueueSend(mensagem, &dado, portMAX_DELAY);
        }
        else {
            dado = 'D';
            xQueueSend(mensagem, &dado, portMAX_DELAY);
        }
        vTaskDelay(20);      
    }
}

void task_lampada()
{
    char comando;
    
    for (;;) {
        xQueueReceive(mensagem, &comando, portMAX_DELAY);
        if (comando == 'L') {
            PORTFbits.RF3 = 1;
            xSemaphoreGive(s);
        }
        else if (comando == 'D') {
            PORTFbits.RF3 = 0;
        }
        vTaskDelay(20);
    }
    
}

void task_speaker()
{
    for (;;) {
        xSemaphoreTake(s, portMAX_DELAY);
        PORTGbits.RG12 = 1;
        vTaskDelay(50);
        PORTGbits.RG12 = 0;
    }    
}

#elif DIP == 1

QueueHandle_t mensagem;
SemaphoreData_t s_contador;

void config_user_app()
{
    // Incializar a fila de mensagens
    mensagem = xQueueCreate(1, sizeof(msg_t))
    
    // Inicializar o semáforo contador
    s_contador = xSemaphoreCreateCounting(4, 0);
    
    // Portas do hardware
}

void task_dip()
{
    for (;;) {
        
    }
}

void task_geral()
{
    for (;;) {
        
    }
}

void task_leds()
{
    for (;;) {
        
    }
}


#endif