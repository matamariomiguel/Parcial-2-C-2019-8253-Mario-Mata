#ifndef CMD_APP_H
#define CMD_APP_H

#include <stdint.h>
#include <stdbool.h>

void Cmd_Init(void);
void Cmd_Service(void);                 /* procesa bytes de RX sin bloquear (llamar en main) */
void Cmd_SendStatus(void);              /* linea de estado: telemetria y STATUS */
bool Cmd_TypingActive(uint32_t nowMs);  /* true si se recibio un byte hace poco (solo pruebas) */

#endif
