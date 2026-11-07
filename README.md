# Segundo Parcial 2L C3-2026:
Monitor de señal analógica con alarma configurable

Firmware con GPIO, TIMER, UART y ADC.

- **Nombre y matrícula:** Mario Miguel, 2019-8253
- **Placa:** FRDM-MCXA156 (MCXA156VLL)
- **Herramientas y versiones:** MCUXpresso IDE v25.6.136, SDK v25.6.136

## Mapa de pines
Ver `Documentos/mapa_pines.md`.

## Esquema de conexiones
[pendiente]

## ADC
- Módulo: ADC1 (LPADC), canal 8, comando 1, disparo por software.
- Referencia de voltaje (VREF): VDDA, 3,3 V nominales (kLPADC_ReferenceVoltageAlt3).
- Resolución N: 12 bits. Rango de lectura: 0 a 4095.
- Conversión: PCT = (100 × ADC + 2047) / 4095, en enteros de 32 bits.
- Entrada: potenciómetro de 10 kΩ con los extremos a 3V3 y GND, y el cursor a la entrada del canal.

## Conexion UART
conexion de 115200 baudios, a 8 bits, sin paridad, de 1 bit de parada, sin control de flujo.

## Cálculo del TIMER
Reloj efectivo: [pendiente] | Prescaler: [pendiente] | Período: [pendiente]

## Compilación y ejecución
1. Abrir el proyecto en MCUXpresso IDE.
2. Compilar en Debug y cargar con la sonda MCU-Link.
3. Abrir la terminal serial a 115200 8N1.

## Resultados de pruebas
Ver `resultados.md` y la carpeta `evidencias/`.
