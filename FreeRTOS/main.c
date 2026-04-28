#include "jantar.h"

void task_1(void);
void task_2(void);

void task_config();

// Estrutura de dados do semáforo
xSemaphoreHandle s;

int main()
{
    // Cria um semáforo binário
    s = xSemaphoreCreateBinary();
    
    xSemaphoreGive(s);
    
    xTaskCreate(task_1, "T1", 128, NULL, 3, NULL);
    xTaskCreate(task_2, "T1", 128, NULL, 2, NULL);
    
    task_config();    
    
    vTaskStartScheduler();
    
    while (1);
    
    return 0;
}

void task_1()
{    
    for (;;) {
        xSemaphoreTake(s, portMAX_DELAY);
        LATDbits.LATD0 = ~PORTDbits.RD0;
    }
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD1;
    }
}

void task_config()
{
    TRISDbits.TRISD0 =  0;
    TRISDbits.TRISD1 =  0;
}
