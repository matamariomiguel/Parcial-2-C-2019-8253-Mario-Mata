#include "alarm_app.h"
#include "app_config.h"
#include "led_app.h"

static bool    s_run     = true;
static uint8_t s_thr     = APP_THR_DEFAULT;
static bool    s_have    = false;      /* ya hay una muestra valida */
static uint8_t s_lastPct = 0U;
static bool    s_alarm   = false;

static void Apply(bool on)
{
    s_alarm = on;
    Led_SetAlarm(on);
}

/* Reinicia la decision: encendida si la ultima muestra cumple PCT >= THR */
static void Reevaluate(void)
{
    if (!s_run)       { Apply(false); return; }
    if (!s_have)      { return; }                 /* espera la primera muestra */
    Apply(s_lastPct >= s_thr);
}

void Alarm_Init(void)
{
    s_run = true;
    s_thr = APP_THR_DEFAULT;
    s_have = false;
    s_lastPct = 0U;
    Apply(false);
}

void Alarm_OnSample(uint8_t pct)
{
    s_lastPct = pct;
    s_have = true;

    if (!s_run)
    {
        Apply(false);
        return;
    }
    if (pct >= s_thr)
    {
        Apply(true);
    }
    else if (pct < (uint8_t)(s_thr - APP_HYSTERESIS))
    {
        Apply(false);
    }
    /* banda intermedia: conserva el estado */
}

void Alarm_SetRun(bool run)
{
    if (run == s_run) { return; }     /* START en RUN conserva la decision */
    s_run = run;
    Reevaluate();                     /* PAUSE -> apaga; RUN -> reinicia decision */
}

void Alarm_SetThr(uint8_t thr)
{
    s_thr = thr;
    Reevaluate();                     /* en PAUSE mantiene apagada */
}

bool    Alarm_IsRun(void)  { return s_run; }
uint8_t Alarm_GetThr(void) { return s_thr; }
bool    Alarm_IsOn(void)   { return s_alarm; }
