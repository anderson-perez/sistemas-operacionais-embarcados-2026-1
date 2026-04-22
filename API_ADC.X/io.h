#ifndef IO_H
#define	IO_H

#include <xc.h>
#include <stdint.h>

void adc_config(void);
void adc_on(void);
uint16_t adc_read(void);



#endif	/* IO_H */

