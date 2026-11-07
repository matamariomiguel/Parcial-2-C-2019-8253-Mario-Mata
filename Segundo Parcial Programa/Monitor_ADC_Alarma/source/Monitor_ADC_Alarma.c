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

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    Uart_Init();
    (void)Tick_Init();
    Led_Init(Tick_GetMs());

    Uart_Puts("\r\nETAPA 6: LEDs. '1' = alarma ON, '0' = alarma OFF\r\n");

    for (;;)
    {
        uint32_t now = Tick_GetMs();
        uint8_t b;

        Led_ActivityService(now);          /* parpadea cada 500 ms */

        while (Uart_ReadByte(&b))
        {
            if (b == '1')
            {
                Led_SetAlarm(true);
                Uart_Puts("ALARMA ON\r\n");
            }
            else if (b == '0')
            {
                Led_SetAlarm(false);
                Uart_Puts("ALARMA OFF\r\n");
            }
        }
        Uart_TxService();
    }
}
