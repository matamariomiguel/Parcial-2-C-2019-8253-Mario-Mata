#include "led_app.h"
#include "app_config.h"
#include "app_time.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

static bool     s_actOn = false;
static bool     s_almOn = false;
static uint32_t s_actLast = 0U;

static void WriteLed(GPIO_Type *gpio, uint32_t pin, bool on)
{
    GPIO_PinWrite(gpio, pin, on ? APP_LED_ON_LEVEL : APP_LED_OFF_LEVEL);
}

void Led_Init(uint32_t nowMs)
{
    CLOCK_EnableClock(kCLOCK_GatePORT3);
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);

    /* Funcion GPIO (MUX 0) en los pines de los dos LED */
    APP_LED_PORT->PCR[APP_LED_ACT_PIN] = PORT_PCR_MUX(0U);
    APP_LED_PORT->PCR[APP_LED_ALM_PIN] = PORT_PCR_MUX(0U);

    gpio_pin_config_t cfg = {kGPIO_DigitalOutput, APP_LED_OFF_LEVEL};
    GPIO_PortInit(APP_LED_ACT_GPIO);
    GPIO_PinInit(APP_LED_ACT_GPIO, APP_LED_ACT_PIN, &cfg);
    GPIO_PinInit(APP_LED_ALM_GPIO, APP_LED_ALM_PIN, &cfg);

    s_actOn = false;
    s_almOn = false;                          /* alarma apagada tras cada reinicio */
    s_actLast = nowMs;
}

void Led_ActivityService(uint32_t nowMs)
{
    if (Time_Due(nowMs, &s_actLast, APP_ACTIVITY_PERIOD_MS))
    {
        s_actOn = !s_actOn;
        WriteLed(APP_LED_ACT_GPIO, APP_LED_ACT_PIN, s_actOn);
    }
}

void Led_SetAlarm(bool on)
{
    s_almOn = on;
    WriteLed(APP_LED_ALM_GPIO, APP_LED_ALM_PIN, s_almOn);
}

bool Led_GetAlarm(void) { return s_almOn; }
