/* *****************************************************************************
 * File:   drv_ntc.c
 * Author: XX
 *
 * Created on YYYY MM DD
 * 
 * Description: ...
 * 
 **************************************************************************** */

/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "drv_ntc.h"

#include "esp_log.h"
#include "sdkconfig.h"

#include <math.h>

#if CONFIG_DRV_ADC_USE
#include "drv_adc.h"
#endif /* CONFIG_DRV_ADC_USE */

/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */
#define TAG "drv_ntc"

/* *****************************************************************************
 * Constants and Macros Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Enumeration Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Type Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Function-Like Macros
 **************************************************************************** */

/* *****************************************************************************
 * Variables Definitions
 **************************************************************************** */
drv_ntc_e_type_t drv_ntc_type[DRV_NTC_COUNT] = 
{
    DRV_NTC_B25_B85,
    DRV_NTC_B25_B85,
    DRV_NTC_B25_B85,    
};

float drv_ntc_beta_b25_bYY_k[DRV_NTC_COUNT] = 
{
    3977.0,
    3977.0,
    3977.0,
};

float drv_ntc_base_resistance_ohm[DRV_NTC_COUNT] = 
{
    4700.0,
    4700.0,
    4700.0,
};

float drv_ntc_pull_up_resistance_ohm[DRV_NTC_COUNT] = 
{
    10000.0,
    10000.0,
    10000.0,
};

#if CONFIG_DRV_ADC_USE
drv_adc_e_analog_input_t drv_ntc_analog_input[DRV_NTC_COUNT] = 
{
    DRV_ADC_AIN_0,
    DRV_ADC_AIN_1,
    DRV_ADC_AIN_2,
};
float drv_ntc_calc_resistance_ohm[DRV_NTC_COUNT] = 
{
    0.0,
    0.0,
    0.0,
};

#endif /* CONFIG_DRV_ADC_USE */

float drv_ntc_pull_down_resistance_ohm[DRV_NTC_COUNT] = 
{
    45000.0,
    45000.0,
    INFINITY,
};


float adc_voltage_range_millivolts = 3300.0;

/* *****************************************************************************
 * Prototype of functions definitions
 **************************************************************************** */

/* *****************************************************************************
 * Functions
 **************************************************************************** */
void drv_ntc_init(void)
{
    //esp_log_level_set(TAG, ESP_LOG_DEBUG);
}

void drv_ntc_setup(drv_ntc_e_index_t ntc_index, drv_ntc_e_type_t ntc_type, float beta_b25_bYY_k, float base_resistance_ohm, float pull_up_resistance_ohm, float pull_down_resistance_ohm)
{
    drv_ntc_type[ntc_index] = ntc_type;
    drv_ntc_beta_b25_bYY_k[ntc_index] = beta_b25_bYY_k;
    drv_ntc_base_resistance_ohm[ntc_index] = base_resistance_ohm;
    drv_ntc_pull_up_resistance_ohm[ntc_index] = pull_up_resistance_ohm;
    drv_ntc_pull_down_resistance_ohm[ntc_index] = pull_down_resistance_ohm;
}

float drv_ntc_get_temperature_from_millivolts(drv_ntc_e_index_t ntc_index, int value_millivolts)
{
    if (ntc_index >= DRV_NTC_COUNT) return -200.0;
    float temperature = 0.0;
    float resistance_ntc = 0.0;
    float resistance_ratio = 0.0;
    float log_resistance_ratio = 0.0;

    float beta_b25_bYY_k = drv_ntc_beta_b25_bYY_k[ntc_index];
    ESP_LOGD(TAG, "beta_b25_bYY_k = %f", beta_b25_bYY_k);

    float base_resistance_ohm = drv_ntc_base_resistance_ohm[ntc_index];
    ESP_LOGD(TAG, "base_resistance_ohm = %f", base_resistance_ohm);

    float pull_up_resistance_ohm = drv_ntc_pull_up_resistance_ohm[ntc_index];
    ESP_LOGD(TAG, "pull_up_resistance_ohm = %f", pull_up_resistance_ohm);

    float voltage_range_millivolts = adc_voltage_range_millivolts;
    ESP_LOGD(TAG, "voltage_range_millivolts = %f", voltage_range_millivolts);

    float pull_down_resistance_ohm = drv_ntc_pull_down_resistance_ohm[ntc_index];
    ESP_LOGD(TAG, "pull_down_resistance_ohm = %f", pull_down_resistance_ohm);



    //calculate resistance from millivolts
    ESP_LOGD(TAG, "value_millivolts = %d", value_millivolts);
    resistance_ntc = value_millivolts * pull_up_resistance_ohm / (voltage_range_millivolts - value_millivolts);
    ESP_LOGD(TAG, "resistance_ntc = %f", resistance_ntc);
    if (pull_down_resistance_ohm != INFINITY)
    {
        float resistance_total = resistance_ntc;
        resistance_ntc = resistance_total * pull_down_resistance_ohm / (pull_down_resistance_ohm - resistance_total);
        ESP_LOGD(TAG, "resistance_ntc fixed without pulldown = %f", resistance_ntc);
    }
    ESP_LOGD(TAG, "resistance_ntc = %f", resistance_ntc);
    resistance_ratio = resistance_ntc / base_resistance_ohm;
    ESP_LOGD(TAG, "resistance_ratio = %f", resistance_ratio);
    log_resistance_ratio = log(resistance_ratio);
    ESP_LOGD(TAG, "log_resistance_ratio = %f", log_resistance_ratio);

    //calculate temperature from resistance ratio
    temperature = (beta_b25_bYY_k / (log_resistance_ratio + beta_b25_bYY_k / 298.15)) - 273.15;
    ESP_LOGD(TAG, "temperature = %f", temperature);

    drv_ntc_calc_resistance_ohm[ntc_index] = resistance_ntc;

    return temperature;
}

#if CONFIG_DRV_ADC_USE
void drv_ntc_setup_analog_input(drv_ntc_e_index_t ntc_index, drv_adc_e_analog_input_t analog_input)
{
    drv_ntc_analog_input[ntc_index] = analog_input;
}
float drv_ntc_get_temperature(drv_ntc_e_index_t ntc_index)
{
    int value_millivolts = 0;
    float temperature = 0.0;

    value_millivolts = drv_adc_get_last_read_data_millivolts(drv_ntc_analog_input[ntc_index]);
    temperature = drv_ntc_get_temperature_from_millivolts(ntc_index, value_millivolts);

    return temperature;
}

float drv_ntc_get_resistance(drv_ntc_e_index_t ntc_index)
{
    if (ntc_index >= DRV_NTC_COUNT) return 0.0;
    float resistance_ohm = 0.0;
    resistance_ohm = drv_ntc_calc_resistance_ohm[ntc_index];
    return resistance_ohm;
}

#endif /* CONFIG_DRV_ADC_USE */
