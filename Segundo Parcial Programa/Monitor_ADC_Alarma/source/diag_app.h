#ifndef DIAG_APP_H
#define DIAG_APP_H

#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

/* Traza de diagnostico para la prueba P02 (solo compilacion de pruebas).
 * Registra el instante de cada muestra ADC y de cada cambio del LED de actividad
 * y, al cumplirse DIAG_WINDOW_MS, imprime un resumen por UART.
 * En la entrega final se deja APP_DIAG_TRACE en 0U y no genera codigo. */

#if (APP_DIAG_TRACE == 1U)
void Diag_Init(uint32_t nowMs);
void Diag_Adc(uint32_t nowMs);       /* llamar con cada muestra ADC nueva */
void Diag_Led(uint32_t nowMs);       /* llamar en cada cambio del LED de actividad */
void Diag_Service(uint32_t nowMs);   /* llamar en el lazo principal */
#else
#define Diag_Init(t)     ((void)(t))
#define Diag_Adc(t)      ((void)(t))
#define Diag_Led(t)      ((void)(t))
#define Diag_Service(t)  ((void)(t))
#endif

#endif
