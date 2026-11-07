# Mapa de pines

| Función | Puerto/pin | Conector | Periférico/canal | Notas |
|---|---|---|---|---|
| Entrada ADC (cursor del potenciómetro) | P1_10 | J4 pin 2 (Arduino A0) | ADC1, canal 8 (ADC1_A8) | Función analógica, buffer digital deshabilitado, sin pull |
| Pulsador SW2 | P1_7 | Integrado en la placa | GPIO1 | Activo en bajo, pull-up interno |
| LED de actividad (verde) | Pin del LED verde del RGB (board.h) | Integrado | GPIO3 | Cambia cada 500 ms |
| LED de alarma (rojo) | P3_12 | Integrado | GPIO3 | Según PCT y THR |
| UART RX | P0_2 | MCU-Link (USB) | LPUART0, función alterna 2 | 115200 8N1 |
| UART TX | P0_3 | MCU-Link (USB) | LPUART0, función alterna 2 | 115200 8N1 |
| Base de tiempo | No usa pin | No aplica | CTIMER0, coincidencia 0 | Tick de 1 ms |
