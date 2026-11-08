#include "diag_app.h"

#if (APP_DIAG_TRACE == 1U)

#include "uart_app.h"

#define DIAG_WINDOW_MS  10000U      /* ventana de observacion (P02: 10 s) */

typedef struct
{
    uint32_t n;       /* eventos registrados */
    uint32_t first;   /* instante del primero (ms) */
    uint32_t last;    /* instante del ultimo (ms) */
    uint32_t dmin;    /* periodo minimo (ms) */
    uint32_t dmax;    /* periodo maximo (ms) */
} DiagStat;

static DiagStat s_adc;
static DiagStat s_led;
static uint32_t s_start = 0U;
static bool     s_done  = false;

static void Add(DiagStat *s, uint32_t t)
{
    if (s->n == 0U)
    {
        s->first = t;
    }
    else
    {
        uint32_t d = (uint32_t)(t - s->last);
        if ((s->n == 1U) || (d < s->dmin)) { s->dmin = d; }
        if ((s->n == 1U) || (d > s->dmax)) { s->dmax = d; }
    }
    s->last = t;
    s->n++;
}

static void Report(const char *name, const DiagStat *s)
{
    uint32_t span  = (uint32_t)(s->last - s->first);
    uint32_t avg10 = (s->n > 1U) ? ((span * 10U) / (s->n - 1U)) : 0U;   /* decimas de ms */

    Uart_Printf("DIAG %s;n=%lu;dmin=%lu;dmax=%lu;avg=%lu.%lu\r\n",
                name,
                (unsigned long)s->n,
                (unsigned long)s->dmin,
                (unsigned long)s->dmax,
                (unsigned long)(avg10 / 10U),
                (unsigned long)(avg10 % 10U));
}

void Diag_Init(uint32_t nowMs)
{
    s_adc   = (DiagStat){0U, 0U, 0U, 0U, 0U};
    s_led   = (DiagStat){0U, 0U, 0U, 0U, 0U};
    s_start = nowMs;
    s_done  = false;
}

void Diag_Adc(uint32_t nowMs)
{
    if (!s_done) { Add(&s_adc, nowMs); }
}

void Diag_Led(uint32_t nowMs)
{
    if (!s_done) { Add(&s_led, nowMs); }
}

void Diag_Service(uint32_t nowMs)
{
    if (!s_done && ((uint32_t)(nowMs - s_start) >= DIAG_WINDOW_MS))
    {
        s_done = true;
        Report("ADC", &s_adc);     /* se espera n~100, periodo 100 ms */
        Report("LED", &s_led);     /* se espera n~20,  periodo 500 ms */
    }
}

#endif /* APP_DIAG_TRACE */
