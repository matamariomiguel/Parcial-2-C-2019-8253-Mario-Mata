#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"
#include "app_config.h"
#include "app_time.h"
#include "uart_app.h"
#include "tick_app.h"
#include "led_app.h"
#include "button_app.h"
#include "adc_app.h"
#include "alarm_app.h"

static void SendStatus(uint32_t now)
{
    Uart_Printf("t=%lu;ADC=%u;PCT=%u;THR=%u;MODE=%s;ALARM=%u\r\n",
                (unsigned long)now,
                (unsigned)Adc_GetRaw(),
                (unsigned)Adc_RawToPct(Adc_GetRaw()),
                (unsigned)Alarm_GetThr(),
                Alarm_IsRun() ? "RUN" : "PAUSE",
                Alarm_IsOn() ? 1U : 0U);
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    Uart_Init();
    (void)Tick_Init();

    uint32_t t0 = Tick_GetMs();
    Led_Init(t0);
    Button_Init(t0);
    Adc_Init(t0);
    Alarm_Init();

    uint32_t lastTelem = t0;
    Uart_Puts("\r\nETAPA 8-9: ADC + alarma. SW2=RUN/PAUSE, 'u'/'d' = THR +5/-5\r\n");

    for (;;)
    {
        uint32_t now = Tick_GetMs();
        uint8_t b;

        Led_ActivityService(now);

        if (Adc_Service(now))
        {
            Alarm_OnSample(Adc_RawToPct(Adc_GetRaw()));
        }

        if (Button_Service(now))
        {
            Alarm_SetRun(!Alarm_IsRun());
        }

        while (Uart_ReadByte(&b))
        {
            uint8_t thr = Alarm_GetThr();
            if ((b == 'u') && (thr + 5U <= APP_THR_MAX)) { Alarm_SetThr((uint8_t)(thr + 5U)); }
            if ((b == 'd') && (thr >= APP_THR_MIN + 5U)) { Alarm_SetThr((uint8_t)(thr - 5U)); }
        }

        if (Time_Due(now, &lastTelem, APP_TELEMETRY_PERIOD_MS))
        {
            SendStatus(now);
        }
        Uart_TxService();
    }
}
