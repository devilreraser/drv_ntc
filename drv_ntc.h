/* *****************************************************************************
 * File:   drv_ntc.h
 * Author: XX
 *
 * Created on YYYY MM DD
 * 
 * Description: ...
 * 
 **************************************************************************** */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* *****************************************************************************
 * Header Includes
 **************************************************************************** */
#include "sdkconfig.h"
#if CONFIG_DRV_ADC_USE
#include "drv_adc.h"
#endif /* CONFIG_DRV_ADC_USE */

    
/* *****************************************************************************
 * Configuration Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Constants and Macros Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Enumeration Definitions
 **************************************************************************** */
typedef enum
{
    DRV_NTC_0,
    DRV_NTC_1,
    DRV_NTC_2,
    DRV_NTC_3,
    DRV_NTC_4,
    DRV_NTC_5,
    DRV_NTC_6,
    DRV_NTC_7,
    DRV_NTC_COUNT
}drv_ntc_e_index_t;

typedef enum
{
    DRV_NTC_B25_B85,

    DRV_NTC_TYPE_COUNT
}drv_ntc_e_type_t;


/* *****************************************************************************
 * Type Definitions
 **************************************************************************** */

/* *****************************************************************************
 * Function-Like Macro
 **************************************************************************** */

/* *****************************************************************************
 * Variables External Usage
 **************************************************************************** */ 

/* *****************************************************************************
 * Function Prototypes
 **************************************************************************** */
void drv_ntc_init(void);
void drv_ntc_setup(drv_ntc_e_index_t ntc_index, drv_ntc_e_type_t ntc_type, float beta_b25_bYY_k, float base_resistance_ohm, float pull_up_resistance_ohm, float pull_down_resistance_ohm);
float drv_ntc_get_temperature_from_millivolts(drv_ntc_e_index_t ntc_index, int value_millivolts);
#if CONFIG_DRV_ADC_USE
void drv_ntc_setup_analog_input(drv_ntc_e_index_t ntc_index, drv_adc_e_analog_input_t analog_input);
float drv_ntc_get_temperature(drv_ntc_e_index_t ntc_index);
float drv_ntc_get_resistance(drv_ntc_e_index_t ntc_index);
#endif /* CONFIG_DRV_ADC_USE */


#ifdef __cplusplus
}
#endif /* __cplusplus */


