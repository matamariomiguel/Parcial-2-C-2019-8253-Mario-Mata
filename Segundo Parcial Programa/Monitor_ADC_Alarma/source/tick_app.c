#include "tick_app.h"
#include "app_config.h"
#include "fsl_ctimer.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_common.h"

static volatile uint32_t g_ms = 0U;

/* Se ejecuta dentro de la ISR del driver (ya limpia el flag). Solo cuenta. */
static void Tick_Callback(uint32_t flags)
{
    if (flags & kCTIMER_Match0Flag)
    {
        ++g_ms;
    }
}

static ctimer_callback_t s_tickCallbacks[] = { Tick_Callback };

uint32_t Tick_Init(void)
{
    ctimer_config_t cfg;
    ctimer_match_config_t matchCfg = {0};

    /* Reloj funcional de CTIMER0 y liberacion del reset */
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1U);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);
    RESET_ReleasePeripheralReset(kCTIMER0_RST_SHIFT_RSTn);

    uint32_t timerClockHz = CLOCK_GetCTimerClkFreq(0U);

    CTIMER_GetDefaultConfig(&cfg);
    cfg.prescale = 0U;
    CTIMER_Init(APP_TICK_TIMER, &cfg);

    /* Con reset en match, el periodo es MR + 1, por eso "- 1" */
    matchCfg.matchValue         = (timerClockHz / 1000U) - 1U;
    matchCfg.enableCounterReset = true;
    matchCfg.enableCounterStop  = false;
    matchCfg.outControl         = kCTIMER_Output_NoAction;
    matchCfg.outPinInitState    = false;
    matchCfg.enableInterrupt    = true;

    CTIMER_RegisterCallBack(APP_TICK_TIMER, s_tickCallbacks, kCTIMER_SingleCallback);
    CTIMER_SetupMatch(APP_TICK_TIMER, kCTIMER_Match_0, &matchCfg);

    EnableIRQ(APP_TICK_IRQn);
    CTIMER_StartTimer(APP_TICK_TIMER);

    return timerClockHz;
}

uint32_t Tick_GetMs(void)
{
    uint32_t msVal;
    uint32_t irqState = DisableGlobalIRQ();
    msVal = g_ms;
    EnableGlobalIRQ(irqState);
    return msVal;
}
