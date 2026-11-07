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

int main(void)
{
    bool run = true;                       /* arranca en RUN */

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    Uart_Init();
    (void)Tick_Init();
    Led_Init(Tick_GetMs());
    Button_Init(Tick_GetMs());

    Uart_Puts("\r\nETAPA 7: boton SW2 alterna RUN/PAUSE. '1'/'0' = alarma ON/OFF\r\n");

    for (;;)
    {
        uint32_t now = Tick_GetMs();
        uint8_t b;

        Led_ActivityService(now);

        if (Button_Service(now))
        {
            run = !run;
            if (!run)
            {
                Led_SetAlarm(false);       /* en PAUSE la alarma queda apagada */
            }
            Uart_Printf("t=%lu SW2 -> %s\r\n", (unsigned long)now, run ? "RUN" : "PAUSE");
        }

        while (Uart_ReadByte(&b))
        {
            if (b == '1')      { Led_SetAlarm(true);  Uart_Puts("ALARMA ON\r\n"); }
            else if (b == '0') { Led_SetAlarm(false); Uart_Puts("ALARMA OFF\r\n"); }
        }
        Uart_TxService();
    }
}
