#include "uart_app.h"
#include "app_config.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"

#define RX_SIZE  256U              /* potencia de 2 */
#define RX_MASK  (RX_SIZE - 1U)

static uint8_t           s_rx[RX_SIZE];
static volatile uint32_t s_rxHead = 0U;   /* escribe la ISR */
static volatile uint32_t s_rxTail = 0U;   /* escribe main   */
static volatile uint32_t s_rxLostFlag = 0U;
static volatile uint32_t s_rxLost = 0U;
static volatile uint32_t s_rxOverrun = 0U;

/* ISR: solo recibe, limpia errores y guarda en el buffer circular */
void APP_UART_IRQHandler(void)
{
    uint32_t flags = LPUART_GetStatusFlags(APP_UART);
    uint32_t err = flags & (kLPUART_RxOverrunFlag | kLPUART_NoiseErrorFlag |
                            kLPUART_FramingErrorFlag | kLPUART_ParityErrorFlag);
    if (err != 0U)
    {
        if (err & kLPUART_RxOverrunFlag)
        {
            s_rxOverrun++;
            s_rxLostFlag = 1U;
        }
        LPUART_ClearStatusFlags(APP_UART, err);
    }

    while (LPUART_GetStatusFlags(APP_UART) & kLPUART_RxDataRegFullFlag)
    {
        uint8_t  b    = LPUART_ReadByte(APP_UART);
        uint32_t next = (s_rxHead + 1U) & RX_MASK;
        if (next != s_rxTail)
        {
            s_rx[s_rxHead] = b;
            s_rxHead = next;
        }
        else
        {
            s_rxLost++;            /* buffer lleno: descarta el byte */
            s_rxLostFlag = 1U;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

void Uart_Init(void)
{
    s_rxHead = s_rxTail = 0U;
    LPUART_ClearStatusFlags(APP_UART, kLPUART_RxOverrunFlag | kLPUART_NoiseErrorFlag |
                                      kLPUART_FramingErrorFlag | kLPUART_ParityErrorFlag);
    LPUART_EnableInterrupts(APP_UART, kLPUART_RxDataRegFullInterruptEnable |
                                      kLPUART_RxOverrunInterruptEnable);
    EnableIRQ(APP_UART_IRQn);
}

bool Uart_ReadByte(uint8_t *b)
{
    if (s_rxTail == s_rxHead)
    {
        return false;
    }
    *b = s_rx[s_rxTail];
    s_rxTail = (s_rxTail + 1U) & RX_MASK;
    return true;
}

bool Uart_TakeRxLost(void)
{
    uint32_t irq = DisableGlobalIRQ();
    bool lost = (s_rxLostFlag != 0U);
    s_rxLostFlag = 0U;
    EnableGlobalIRQ(irq);
    return lost;
}

uint32_t Uart_RxOverrunCount(void) { return s_rxOverrun; }
uint32_t Uart_RxLostCount(void)    { return s_rxLost; }
