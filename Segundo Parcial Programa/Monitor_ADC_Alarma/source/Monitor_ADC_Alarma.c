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
#include "cmd_app.h"

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
    Cmd_Init();

    uint32_t lastTelem = t0;

    for (;;)
    {
        uint32_t now = Tick_GetMs();

        Led_ActivityService(now);                       /* 500 ms */

        if (Adc_Service(now))                           /* 100 ms */
        {
            Alarm_OnSample(Adc_RawToPct(Adc_GetRaw()));
        }

        if (Button_Service(now))
        {
            Alarm_SetRun(!Alarm_IsRun());
        }

        Cmd_Service();

        if (Time_Due(now, &lastTelem, APP_TELEMETRY_PERIOD_MS))   /* 1000 ms */
        {
            if (!Cmd_TypingActive(now))
            {
                Cmd_SendStatus();
            }
        }

        Uart_TxService();
    }
}
