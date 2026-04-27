#include "FreeRTOS.h"
#include "task.h"

void task_1();
void task_2();

void task_config();


int main()
{
    xTaskCreate(task_1, "T1", 128, NULL, 2, NULL);
    xTaskCreate(task_2, "T1", 128, NULL, 2, NULL);
    
    task_config();
    
    vTaskStartScheduler();
    
    while (1);
    
    return 0;
}

void task_1()
{
    for (;;) {
        LATDbits.LATD0 = ~PORTDbits.RD0;
    }
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD0;
    }
}

void task_config()
{
    TRISDbits.TRISD0 =  0;
    TRISDbits.TRISD1 =  0;
}
