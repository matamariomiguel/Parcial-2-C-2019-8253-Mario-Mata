#ifndef LED_APP_H
#define LED_APP_H

#include <stdint.h>
#include <stdbool.h>

void Led_Init(uint32_t nowMs);
void Led_ActivityService(uint32_t nowMs);   /* alterna el LED de actividad cada 500 ms */
void Led_SetAlarm(bool on);                 /* LED de alarma */
bool Led_GetAlarm(void);

#endif
