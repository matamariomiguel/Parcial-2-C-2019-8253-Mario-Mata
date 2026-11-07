#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "app_config.h"
#include "app_time.h"
#include "uart_app.h"

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();            /* solo deja pines y velocidad de la LPUART */

    Uart_Init();                         /* RX por interrupcion + TX encolado */
    Uart_Puts("\r\nPaso 4: UART OK. Escriba algo.\r\n");

    for (;;)
    {
        uint8_t b;

        while (Uart_ReadByte(&b))        /* eco de lo recibido */
        {
            char s[2] = { (char)b, '\0' };
            Uart_Puts(s);
        }
        Uart_TxService();                /* vaciar cola TX sin bloquear */
    }
}
