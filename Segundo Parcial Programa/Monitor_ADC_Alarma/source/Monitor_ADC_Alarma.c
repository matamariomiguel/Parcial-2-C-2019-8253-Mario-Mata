#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "app_config.h"
#include "app_time.h"
#include "uart_app.h"
#include "tick_app.h"

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    Uart_Init();
    uint32_t tickClk = Tick_Init();
    Uart_Printf("\r\nPaso 5: tick de 1 ms. CTIMER0=%lu Hz\r\n", (unsigned long)tickClk);

    uint32_t last = Tick_GetMs();

    for (;;)
    {
        uint32_t now = Tick_GetMs();
        uint8_t b;

        if (Time_Due(now, &last, 1000U))     /* una linea por segundo */
        {
            Uart_Printf("ms=%lu\r\n", (unsigned long)now);
        }

        while (Uart_ReadByte(&b))            /* el eco sigue funcionando */
        {
            char s[2] = { (char)b, '\0' };
            Uart_Puts(s);
        }
        Uart_TxService();
    }
}
