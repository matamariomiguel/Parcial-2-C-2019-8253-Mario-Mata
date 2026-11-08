#ifndef ALARM_APP_H
#define ALARM_APP_H

#include <stdint.h>
#include <stdbool.h>

void    Alarm_Init(void);
void    Alarm_OnSample(uint8_t pct);        /* llamar con cada muestra nueva */
void    Alarm_SetRun(bool run);             /* START/STOP/boton */
void    Alarm_SetThr(uint8_t thr);          /* cambio de umbral */
bool    Alarm_IsRun(void);
uint8_t Alarm_GetThr(void);
bool    Alarm_IsOn(void);

#endif
