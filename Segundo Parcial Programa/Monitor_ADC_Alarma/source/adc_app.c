#include "adc_app.h"
#include "app_config.h"
#include "app_time.h"
#include "fsl_lpadc.h"
#include "fsl_clock.h"
#include "fsl_reset.h"

static uint32_t s_last    = 0U;
static bool     s_pending = false;
static bool     s_valid   = false;
static uint16_t s_raw     = 0U;

void Adc_Init(uint32_t nowMs)
{
    lpadc_config_t              cfg;
    lpadc_conv_command_config_t cmd;
    lpadc_conv_trigger_config_t trg;

    /* Pin analogico P1_10 (ADC1_A8): MUX 0, sin buffer de entrada digital */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    APP_ADC_PORT->PCR[APP_ADC_PIN] = PORT_PCR_MUX(0U);

    /* Reloj funcional del ADC1: FRO 12 MHz, divisor 1 (igual que el ejemplo lpadc_polling) */
    RESET_ReleasePeripheralReset(kADC1_RST_SHIFT_RSTn);
    CLOCK_SetClockDiv(kCLOCK_DivADC1, 1U);
    CLOCK_AttachClk(kFRO12M_to_ADC1);

    LPADC_GetDefaultConfig(&cfg);
#if defined(FSL_FEATURE_LPADC_HAS_CFG_PWRSEL) && (FSL_FEATURE_LPADC_HAS_CFG_PWRSEL == 1U)
    cfg.powerLevelMode = kLPADC_PowerLevelAlt4;
#endif
    cfg.enableAnalogPreliminary = true;
    cfg.referenceVoltageSource  = kLPADC_ReferenceVoltageAlt3;   /* VDDA */
    LPADC_Init(APP_ADC_BASE, &cfg);
    LPADC_DoAutoCalibration(APP_ADC_BASE);

    /* Comando 1: canal 8, lado A, 12 bits, promedio de 16 muestras */
    LPADC_GetDefaultConvCommandConfig(&cmd);
    cmd.sampleChannelMode        = kLPADC_SampleChannelSingleEndSideA;
    cmd.channelNumber            = APP_ADC_CHANNEL;
    cmd.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
    cmd.hardwareAverageMode      = kLPADC_HardwareAverageCount16;
    cmd.sampleTimeMode           = kLPADC_SampleTimeADCK131;
    LPADC_SetConvCommandConfig(APP_ADC_BASE, APP_ADC_CMD_ID, &cmd);

    /* Disparador 0 por software, ejecuta el comando 1 */
    LPADC_GetDefaultConvTriggerConfig(&trg);
    trg.targetCommandId = APP_ADC_CMD_ID;
    LPADC_SetConvTriggerConfig(APP_ADC_BASE, APP_ADC_TRIGGER_ID, &trg);

    s_last    = nowMs;
    s_pending = false;
    s_valid   = false;
}

bool Adc_Service(uint32_t nowMs)
{
    lpadc_conv_result_t r;

    if (Time_Due(nowMs, &s_last, APP_SAMPLE_PERIOD_MS))
    {
        LPADC_DoSoftwareTrigger(APP_ADC_BASE, 1U << APP_ADC_TRIGGER_ID);
        s_pending = true;
    }

    if (s_pending && LPADC_GetConvResult(APP_ADC_BASE, &r))
    {
        uint32_t v = ((uint32_t)r.convValue >> APP_ADC_RESULT_SHIFT) & APP_ADC_MAX;
        s_raw     = (uint16_t)v;
        s_valid   = true;
        s_pending = false;
        return true;
    }
    return false;
}

bool     Adc_HasSample(void) { return s_valid; }
uint16_t Adc_GetRaw(void)    { return s_raw; }

/* PCT = redondear(100 * ADC / 4095) en enteros de 32 bits (max 409500, sin desbordar) */
uint8_t Adc_RawToPct(uint16_t raw)
{
    uint32_t x = raw;
    if (x > APP_ADC_MAX) { x = APP_ADC_MAX; }
    return (uint8_t)((200U * x + APP_ADC_MAX) / (2U * APP_ADC_MAX));
}
