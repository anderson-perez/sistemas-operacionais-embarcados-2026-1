#include "jantar.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOS.h"

xSemaphoreHandle hashis[T];

/**
 * Pendência:
 * Concluir o exercício do problema do Jantar dos Filósofos.
 * 
 */

void jantar_config()
{
    
    // Fazer tris
    
    
    for (int i = 0; i < T; i++) {
        hashis[i] = xSemaphoreCreateBinary();
        xSemaphoreGive(hashis[i]);
    }
}

void fil_0()
{
    while (1) {
        xSemaphoreTake(hashis[0], portMAX_DELAY);
        xSemaphoreTake(hashis[1], portMAX_DELAY);
        
        // Acende leds
        // Fica aceso por um tempo
        
        // Apaga leds
        
        // Libera semáforos
        xSemaphoreGive(hashis[0]);
        xSemaphoreGive(hashis[1]);
        
    }
}
    
void fil_1()
{
    while (1) {
        
    }    
}

void fil_2()
{
    while (1) {
        
    }    
}

void fil_3()
{
    while (1) {
        
    }    
}

void fil_4()
{
    while (1) {
        
    }    
}

