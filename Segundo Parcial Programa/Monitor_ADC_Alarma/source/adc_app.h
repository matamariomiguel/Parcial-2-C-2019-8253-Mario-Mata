#ifndef ADC_APP_H
#define ADC_APP_H

#include <stdint.h>
#include <stdbool.h>

void     Adc_Init(uint32_t nowMs);
/* Dispara una conversion cada 100 ms y recoge el resultado sin esperar.
 * Devuelve true cuando hay una muestra nueva. */
bool     Adc_Service(uint32_t nowMs);
bool     Adc_HasSample(void);
uint16_t Adc_GetRaw(void);                    /* ultima lectura, 0..4095 */
uint8_t  Adc_RawToPct(uint16_t raw);          /* 0..100, redondeado */

#endif
