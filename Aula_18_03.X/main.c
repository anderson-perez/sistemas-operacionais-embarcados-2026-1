#include <xc.h>
#include <stdint.h>

typedef void (*f_ptr)(void);

typedef enum {READY = 1, RUNNING, WAITING} state_t;

typedef struct tcb {
    uint8_t id;
    f_ptr task_f;
    uint8_t delay;
    state_t task_state;
} tcb_t;

void config_timer0();
void create_task(f_ptr f, uint8_t id);
f_ptr round_robin();
void config_tasks();

void __interrupt() ISR();

void task_1();
void task_2();
void task_3();

tcb_t ready_queue[3];
uint8_t pos = 0, vez = 0;

asm("global _task_1, _task_2, _task_3");

int main()
{
    config_tasks();
    
    
    // Criar tarefas
    create_task(task_1, 1);
    create_task(task_2, 2);
    create_task(task_3, 3);
    
    config_timer0();
    
    while (1) {
        
    }
    
    return 0;
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

void create_task(f_ptr f, uint8_t id)
{
    ready_queue[pos].id         = id;
    ready_queue[pos].task_f     = f;
    ready_queue[pos].delay      = 0;
    ready_queue[pos++].task_state = READY;
}

void __interrupt() ISR()
{
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        TMR0 = 0;
        // Chama o escalonador
        STKPTR = 0;
        asm("push");
        TOS = (uint24_t)round_robin();
    }
        
}

void task_1()
{
    while (1) {
        LATDbits.LATD0 = ~PORTDbits.RD0;
    }
    
}

void task_2()
{
    while (1) {
        LATDbits.LATD1 = ~PORTDbits.RD1;
    }
}

void task_3()
{
    while (1) {
        LATDbits.LATD2 = ~PORTDbits.RD2;
    }
}

f_ptr round_robin()
{
    do {        
        if (ready_queue[vez].task_state == READY) {
            vez = (vez+1) % 3;
            return ready_queue[vez].task_f;
        }
        vez = (vez+1) % 3;
    } while (ready_queue[vez].task_state != READY);
    
    return NULL;
}

void config_tasks()
{
    TRISDbits.RD0       = 0;
    TRISDbits.RD1       = 0;
    TRISDbits.RD2       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELDbits.ANSD1    = 0;
    ANSELDbits.ANSD2    = 0;
}
