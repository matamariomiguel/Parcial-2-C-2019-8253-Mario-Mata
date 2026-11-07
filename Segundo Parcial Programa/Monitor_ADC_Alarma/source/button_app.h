#ifndef BUTTON_APP_H
#define BUTTON_APP_H

#include <stdint.h>
#include <stdbool.h>

void Button_Init(uint32_t nowMs);
/* true UNA sola vez por pulsacion valida (30 ms estable) */
bool Button_Service(uint32_t nowMs);

#endif
