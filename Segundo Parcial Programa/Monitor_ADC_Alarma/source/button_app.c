#include "button_app.h"
#include "app_config.h"
#include "app_time.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

static bool     s_rawPressed    = false;   /* ultima lectura cruda */
static bool     s_stablePressed = false;   /* estado ya validado   */
static uint32_t s_changeMs      = 0U;      /* instante del ultimo cambio crudo */

static bool ReadRaw(void)
{
    return (GPIO_PinRead(APP_BTN_GPIO, APP_BTN_PIN) == 0U);   /* activo en bajo */
}

void Button_Init(uint32_t nowMs)
{
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);

    /* GPIO, pull-up interno (PE+PS), buffer de entrada habilitado */
    APP_BTN_PORT->PCR[APP_BTN_PIN] = PORT_PCR_MUX(0U) | PORT_PCR_PE_MASK |
                                     PORT_PCR_PS_MASK | PORT_PCR_IBE_MASK;

    gpio_pin_config_t cfg = {kGPIO_DigitalInput, 0U};
    GPIO_PortInit(APP_BTN_GPIO);
    GPIO_PinInit(APP_BTN_GPIO, APP_BTN_PIN, &cfg);

    s_rawPressed    = ReadRaw();
    s_stablePressed = s_rawPressed;        /* si arranca presionado, no genera evento */
    s_changeMs      = nowMs;
}

bool Button_Service(uint32_t nowMs)
{
    bool raw = ReadRaw();

    if (raw != s_rawPressed)               /* cambio: reinicia la ventana de 30 ms */
    {
        s_rawPressed = raw;
        s_changeMs   = nowMs;
        return false;
    }

    if ((raw != s_stablePressed) &&
        (Time_Elapsed(nowMs, s_changeMs) >= APP_DEBOUNCE_MS))
    {
        s_stablePressed = raw;
        return raw;                        /* solo la pulsacion genera evento; la liberacion valida y rearma */
    }
    return false;
}
