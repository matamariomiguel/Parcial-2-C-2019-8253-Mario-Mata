#include "cmd_app.h"
#include "app_config.h"
#include "uart_app.h"
#include "tick_app.h"
#include "adc_app.h"
#include "alarm_app.h"
#include <string.h>

/* Hasta 33 bytes (32 de linea + un posible CR) y 1 para el NUL */
#define BUF_SZ  (APP_LINE_MAX + 2U)

static char     s_buf[BUF_SZ];
static uint32_t s_len      = 0U;
static bool     s_overflow = false;
static bool     s_lastCR   = false;     /* el byte anterior fue CR */

void Cmd_SendStatus(void)
{
    uint16_t raw = Adc_GetRaw();
    Uart_Printf("t=%lu;ADC=%u;PCT=%u;THR=%u;MODE=%s;ALARM=%u\r\n",
                (unsigned long)Tick_GetMs(),
                (unsigned)raw,
                (unsigned)Adc_RawToPct(raw),
                (unsigned)Alarm_GetThr(),
                Alarm_IsRun() ? "RUN" : "PAUSE",
                Alarm_IsOn() ? 1U : 0U);
}

/* true y el valor si 's' son 1 a 3 digitos decimales y nada mas */
static bool ParseUInt(const char *s, uint32_t *out)
{
    uint32_t v = 0U;
    uint32_t n = 0U;

    while ((s[n] >= '0') && (s[n] <= '9'))
    {
        v = (v * 10U) + (uint32_t)(s[n] - '0');
        n++;
        if (n > 3U) { return false; }
    }
    if ((n == 0U) || (s[n] != '\0')) { return false; }
    *out = v;
    return true;
}

static void ProcessLine(const char *line)
{
    uint32_t v;

    if (strcmp(line, "STATUS") == 0)
    {
        Cmd_SendStatus();
    }
    else if (strcmp(line, "START") == 0)
    {
        Alarm_SetRun(true);                 /* si ya esta en RUN conserva la decision */
        Uart_Puts("OK RUN\r\n");
    }
    else if (strcmp(line, "STOP") == 0)
    {
        Alarm_SetRun(false);                /* PAUSE: alarma apagada */
        Uart_Puts("OK PAUSE\r\n");
    }
    else if ((strncmp(line, "THR ", 4U) == 0) && ParseUInt(&line[4], &v) &&
             (v >= APP_THR_MIN) && (v <= APP_THR_MAX))
    {
        Alarm_SetThr((uint8_t)v);
        Uart_Printf("OK THR=%u\r\n", (unsigned)v);
    }
    else
    {
        Uart_Puts("ERR\r\n");               /* no cambia ningun parametro */
    }
}

static void EndOfLine(void)
{
    bool bad = s_overflow;

    if (!bad)
    {
        if ((s_len > 0U) && (s_buf[s_len - 1U] == '\r'))
        {
            s_len--;                        /* el CR de CRLF no cuenta */
        }
        if (s_len > APP_LINE_MAX)
        {
            bad = true;                     /* mas de 32 caracteres */
        }
    }

    if (bad)
    {
        Uart_Puts("ERR\r\n");
    }
    else if (s_len > 0U)                    /* linea vacia: se ignora */
    {
        bool printable = true;
        s_buf[s_len] = '\0';
        for (uint32_t i = 0U; i < s_len; i++)
        {
            if (((uint8_t)s_buf[i] < 0x20U) || ((uint8_t)s_buf[i] > 0x7EU))
            {
                printable = false;
            }
        }
        if (printable) { ProcessLine(s_buf); }
        else           { Uart_Puts("ERR\r\n"); }
    }

    s_len      = 0U;
    s_overflow = false;
}

void Cmd_Init(void)
{
    s_len      = 0U;
    s_overflow = false;
    s_lastCR   = false;
}

void Cmd_Service(void)
{
    uint8_t b;

    if (Uart_TakeRxLost())
    {
        s_overflow = true;                  /* se perdieron bytes: la linea actual no es fiable */
    }

    while (Uart_ReadByte(&b))
    {
#if (APP_ACCEPT_CR_EOL == 1U)
        if (b == '\r')
        {
            s_lastCR = true;
            EndOfLine();                    /* CR cierra la linea (Enter de PuTTY) */
            continue;
        }
        if ((b == '\n') && s_lastCR)
        {
            s_lastCR = false;               /* LF de un CRLF: ya se proceso con el CR */
            continue;
        }
        s_lastCR = false;
#endif
        if (b == '\n')
        {
            EndOfLine();
        }
        else if (!s_overflow)
        {
            if (s_len < (APP_LINE_MAX + 1U))
            {
                s_buf[s_len++] = (char)b;
            }
            else
            {
                s_overflow = true;          /* demasiado larga: descarta hasta el terminador */
            }
        }
    }
}
