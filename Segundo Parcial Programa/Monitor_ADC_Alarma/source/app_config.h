#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "board.h"
#include "fsl_device_registers.h"

/* ===== LED de actividad: LED externo en P2_5 (Arduino A1, J4 pin 4), activo en alto ===== */
#define APP_LED_ACT_GPIO       GPIO2
#define APP_LED_ACT_PORT       PORT2
#define APP_LED_ACT_PIN        5U
#define APP_LED_ACT_ON_LEVEL   1U
#define APP_LED_ACT_OFF_LEVEL  0U

/* ===== LED de alarma: rojo del RGB integrado (P3_12), activo en bajo. Pines de board.h ===== */
#define APP_LED_ALM_GPIO       BOARD_LED_RED_GPIO
#define APP_LED_ALM_PORT       PORT3
#define APP_LED_ALM_PIN        BOARD_LED_RED_GPIO_PIN
#define APP_LED_ALM_ON_LEVEL   LOGIC_LED_ON
#define APP_LED_ALM_OFF_LEVEL  LOGIC_LED_OFF

/* ===== Pulsador SW2: activo en bajo, pull-up. Se lee por sondeo con antirrebote de 30 ms ===== */
#define APP_BTN_GPIO          GPIO1
#define APP_BTN_PORT          PORT1
#define APP_BTN_PIN           7U

/* ===== UART de la consola (MCU-Link): P0_2 RX, P0_3 TX ===== */
#define APP_UART              LPUART0
#define APP_UART_IRQn         LPUART0_IRQn
#define APP_UART_IRQHandler   LPUART0_IRQHandler

/* ===== ADC: P1_10 = ADC1_A8, referencia VDDA, 12 bits ===== */
#define APP_ADC_BASE          ADC1
#define APP_ADC_PORT          PORT1
#define APP_ADC_PIN           10U
#define APP_ADC_CHANNEL       8U
#define APP_ADC_CMD_ID        1U
#define APP_ADC_TRIGGER_ID    0U
#define APP_ADC_BITS          12U
#define APP_ADC_MAX           4095U
#define APP_ADC_RESULT_SHIFT  3U   /* el LPADC entrega el dato de 12 bits alineado a 16 */

/* ===== Base de tiempo ===== */
#define APP_TICK_TIMER        CTIMER0
#define APP_TICK_IRQn         CTIMER0_IRQn

/* ===== Parametros de la aplicacion (enunciado) ===== */
#define APP_SAMPLE_PERIOD_MS     100U
#define APP_ACTIVITY_PERIOD_MS   500U
#define APP_TELEMETRY_PERIOD_MS  1000U
#define APP_DEBOUNCE_MS          30U
#define APP_THR_DEFAULT          70U
#define APP_THR_MIN              10U
#define APP_THR_MAX              95U
#define APP_HYSTERESIS           5U
#define APP_LINE_MAX             32U

/* Entrega final: solo LF o CRLF cierran una linea, como pide el enunciado.
 * Para probar a mano con un terminal que envia solo CR, ponerlo en 1U. */
#define APP_ACCEPT_CR_EOL        0U


#endif
