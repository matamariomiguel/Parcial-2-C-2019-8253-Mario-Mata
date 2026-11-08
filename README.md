# Segundo Parcial 2L C3-2026:
Monitor de señal analógica con alarma configurable

Firmware con GPIO, TIMER, UART y ADC.

- **Nombre y matrícula:** Mario Miguel, 2019-8253
- **Placa:** FRDM-MCXA156 (MCXA156VLL)
- **Herramientas y versiones:** MCUXpresso IDE v25.6.136, SDK v25.6.136

## Mapa de pines
Ver `Documentos/mapa_pines.md`.

## Esquema de conexiones
    Potenciómetro 10 kΩ
      extremo 1 ── 3V3 (header Arduino)
      cursor    ── J4 pin 2 (A0, P1_10, ADC1_A8)
      extremo 2 ── GND
   
## Pulsador
- SW2 integrado, pin P1_7 (GPIO1), activo en bajo con pull-up interno; la entrada nunca queda flotante.
- Se lee por sondeo desde el bucle principal, sin retardos bloqueantes: un cambio de nivel reinicia una ventana de 30 ms, y solo si el nivel se mantiene estable ese tiempo se acepta.
- Una pulsación válida alterna RUN y PAUSE una sola vez. Mantener el botón no repite la acción, porque la liberación también se valida antes de aceptar otra pulsación.
    
## ADC
- Módulo: ADC1 (LPADC), canal 8 (pin P1_10), comando 1, disparo por software.
- Reloj: FRO 12 MHz, divisor 1.
- Referencia de voltaje (VREF): VDDA, 3,3 V nominales.
- Resolución N: 12 bits. Rango de lectura: 0 a 4095.
- Conversión: PCT = (100 × ADC + 2047) / 4095, en enteros de 32 bits.
- Entrada: potenciómetro de 10 kΩ con un extremo a 3V3, el otro a GND y el cursor a J4 pin 2.

## UART
- LPUART0 (P0_2 RX, P0_3 TX) a 115200 baudios, 8 bits, sin paridad, 1 bit de parada y sin control de flujo, por el puerto serial virtual del MCU-Link.
- Recepción por interrupción: la ISR solo guarda el byte en un búfer circular de 256 bytes y cuenta los errores; las líneas se procesan fuera de la interrupción.
- Transmisión no bloqueante: cola de 1024 bytes que el bucle principal vacía poco a poco.
- Líneas de hasta 32 caracteres terminadas en LF o CRLF. Una línea más larga se descarta completa, responde ERR y permite procesar la siguiente.
- Comandos (distinguen mayúsculas): STATUS, THR n (10 a 95), START, STOP. Cualquier otro texto responde ERR.

## Cálculo del TIMER
- Periférico: CTIMER0, coincidencia (match) 0 con reinicio del contador e interrupción.
- Reloj efectivo: 96 MHz (FRO_HF con divisor 1), leído en ejecución con CLOCK_GetCTimerClkFreq y mostrado por la UART al arrancar.
- Prescaler: 0.
- Período de 1 ms: 96 000 000 Hz / 1000 = 96 000 ciclos. Valor del registro de coincidencia: 96 000 − 1 = 95 999, porque con reinicio en la coincidencia el período es MR + 1.
- La interrupción solo incrementa un contador de milisegundos. De ese tick salen las tareas periódicas: muestra del ADC cada 100 ms, LED de actividad cada 500 ms y telemetría cada 1000 ms.
- Las diferencias de tiempo se calculan con enteros sin signo, así que resisten el desbordamiento del contador.

## Compilación y ejecución
1. Abrir el proyecto en MCUXpresso IDE.
2. Compilar en Debug y cargar con la sonda MCU-Link.
3. Abrir la terminal serial a 115200 8N1.

## Resultados de pruebas
Ver `resultados.md` y la carpeta `evidencias/`.
