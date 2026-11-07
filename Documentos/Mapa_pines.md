# Mapa de pines

| Función | Puerto/pin | Conector | Notas |
|---|---|---|---|
| Entrada ADC (potenciómetro, cursor) | P1_10 (Arduino A0) | J4 pin 2 | Módulo y canal ADC por confirmar con el ejemplo lpadc_polling |
| Extremos del potenciómetro | 3V3 y GND | J3 pin 7 (3V3) y GND | Por confirmar contra el esquema |
| Pulsador (SW2, wake-up) | P1_7 | Integrado en la placa | Activo en bajo, pull-up interno |
| LED de actividad | LED verde del RGB | Integrado en la placa | Pin según board.h (BOARD_LED_GREEN_*) |
| LED de alarma | LED rojo del RGB | Integrado en la placa | Pin según board.h (BOARD_LED_RED_*) |
| UART (consola) | Instancia de BOARD_DEBUG_UART | MCU-Link (USB) | 115200 8N1, sin control de flujo |
