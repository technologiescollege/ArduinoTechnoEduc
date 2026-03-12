/* -------------------------------------------------------------------------- */
/* FILENAME:
   AUTHOR:      Daniele Aimo (d.aimo@arduino.cc)
   DATE:        20230801
   REVISION:    0.0.1
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                This Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#ifndef OPTA_ANALOG_BLUE_CONFIGURATION
#define OPTA_ANALOG_BLUE_CONFIGURATION

#define FW_VERSION_MAJOR 0
#define FW_VERSION_MINOR 1
#define FW_VERSION_RELEASE 13


#define OPTA_ANALOG_WATCHTDOG_TIME_ms 0xFFFF

/* ~~~~~~~~~~~~~~~~~~~~~~~~ TIMING CONFIGURATION ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* time after the main reset to wait for the reset of Analog Chips */
#define PRE_AN_DEV_RESET_TIME 10
/* time for Analog Chips resets */
#define AN_DEV_RESET_TIME 50
/* time after the reset */
#define POST_AN_DEV_RESET_TIME 100
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
 * "arduino" related configuration defines
 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define OA_NUMBER_OF_LEDS 8
#define OA_VARIANT_FIST_LED_POS 10
/* ######################### PWM Settings ################################## */

/*
 * PWM0_A - ch1 of first PWM IC    P107   [D4]
 * PWM3_A - ch2 of first PWM IC    P111   [D7]
 * IRQ3 - Fault of first PWM IC    P110   [D8]
 *
 * PWM2_A - ch1 of second PWM IC   P500   [D6]
 * PWM1_A - ch2 of second PWM IC   P105   [D5]
 * IRQ11 - Fault of second PWM IC  P501   [D9]
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
 *  related configuration defines
 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define AN_DEV_CHANNELS_NUM 4
#define AN_DEV_DIAG_CHANNELS_NUM 4
#define AN_DEV_GPO_NUM 4


/** The value of the sense resistor in ohms */
#define AN_DEV_RSENSE 100
/** 16 bit ADC */
#define AN_DEV_ADC_MAX_VALUE 65535

/*
 * Register map 
 */

#define OPTA_AN_NOP___SINGLE_PER_DEVICE                  (0x00)

#define OA_REG_FUNC_SETUP                                (0x01)
#define OA_REG_ADC_CONFIG                                (0x05)
#define OA_REG_DIN_CONFIG                                (0x09)

#define OA_REG_GPO_PARALLEL___SINGLE_PER_DEVICE          (0x0D)

#define OA_REG_GPO_CONFIG                                (0x0E)
#define OA_REG_DAC_CONFIG                                (0x12)
#define OA_REG_DAC_CODE                                  (0x16)
#define OA_REC_DAC_CLEAR_CODE                            (0x1A)
#define OA_REG_DAC_ACTIVE                                (0x1E)

#define OA_REG_DIN_THRESH___SINGLE_PER_DEVICE            (0x22)
#define OA_REG_ADC_CONV_CTRL___SINGLE_PER_DEVICE         (0x23)
#define OA_REG_DIAG_SELECT___SINGLE_PER_DEVICE           (0x24)
#define OA_REG_DIN_COMP_OUT___SINGLE_PER_DEVICE          (0x25)

#define OA_REG_ADC_RESULT                                (0x26)
#define OA_REG_DIAG_RESULT                               (0x2A)

#define OA_REG_ALERT_STATUS___SINGLE_PER_DEVICE          (0x2E)
#define OA_REG_LIVE_STATUS___SINGLE_PER_DEVICE           (0x2F)
#define OA_REG_ALERT_MASK___SINGLE_PER_DEVICE            (0x3C)

#define OA_REG_READ_SELECT___SINGLE_PER_DEVICE           (0x41)
#define OA_REG_80_SPS_ACD_CONV___SINGLE_PER_DEVICE       (0x42)
#define OA_REG_THERMAL_RESET_ENABLE___SINGLE_PER_DEVICE  (0x43)
#define OA_REG_CMD_REGISTER___SINGLE_PER_DEVICE          (0x44)
#define OA_REG__SCRATCH___SINGLE_PER_DEVICE              (0x45)
#define OA_REG_SILICON_REV___SINGLE_PER_DEVICE           (0x46)


/* write the following 2 keys into the CMD_REGISTER to reset the device */
#define OPTA_AN_KEY_RESET_1 0x15FA
#define OPTA_AN_KEY_RESET_2 0xAF51
/* write the following KEY to synch DAC output on different channels (must be
 * activate via a specific PIN (not available at the present) */
#define OPTA_AN_KEY_LDAC 0x953A
/* write this key to reset the DAC to the reset Value */
#define OPTA_AN_KEY_CLEAR_DAC 0x73D1



/* -------------------------------------------------------------------------- */

#define OPTA_AN_CH_FUNC_SETUP(x) (0x01 + x)

#define CH_HIGH_IMP 0x0   // ADC
#define CH_VO       0x1   // Voltage Output
#define CH_CO       0x2   // Current output
#define CH_VI       0x3   // Voltage Input
#define CH_CI_EP    0x4   // Current Input Externally Powered
#define CH_CI_LP    0x5   // Current Input Loop Powered
#define CH_RM       0x6   // Resistance Measurement
#define CH_DI       0x7   // Digital Input
#define CH_DI_LP    0x8   // Digital Input Loop Powered

/* -------------------------------------------------------------------------- */
/* ADC configuration                                                          */
/* -------------------------------------------------------------------------- */
#define OPTA_AN_ADC_CONFIG(x) (0x05 + x)

#define ADC_CFG_MUX_100_OHM 0x1
#define ADC_CFG_MUX_AD_SENSE 0x0
#define ADC_CFG_SET_100_OHM(v) (v |= ADC_CFG_MUX_100_OHM)
#define ADC_CFG_SET_AD_SENSE(v) (v &= ~ADC_CFG_MUX_100_OHM)

#define ADC_CFG_PULL_DOWN_YES_BIT (1 << 2)
#define ADC_CFG_ENABLE_PULL_DOWN(v) (v |= ADC_CFG_PULL_DOWN_YES_BIT)
#define ADC_CFG_DISABLE_PULL_DOWN(v) (v &= ~ADC_CFG_PULL_DOWN_YES_BIT)

#define ADC_CFG_ENABLE_REJECT(v) (v = v)
#define ADC_CFG_DISABLE_REJECT(v) (v |= (1 << 3))

#define ADC_CFG_RANGE_10V (0b00 << 5)
#define ADC_CFG_RANGE_2_5V_RTD (0b01 << 5)
#define ADC_CFG_RANGE_2_5V_LOOP (0b10 << 5)
#define ADC_CFG_RANGE_2_5V_BI (0b11 << 5)
#define ADC_CFG_RANGE_MASK (0b11 << 5)

#define ADC_CFG_RESET_RANGE(v) (v &= ~ADC_CFG_RANGE_MASK)
#define ADC_CFG_SET_RANGE(v, x) (v |= x)

#define ADC_DATA_READY (0x4000) // 14th bit

#define ADC_BUSY_BIT 13
#define ADC_BUSY_MASK (1 << ADC_BUSY_BIT)
/* -------------------------------------------------------------------------- */
/* DIGITAL INPUT CONFIGURATION                                                */
/* -------------------------------------------------------------------------- */
#define OPTA_AN_DIN_CONFIG(x) (0x09 + x)

#define DEBOUNCE_TIME(v, x) (v |= (x & 0x1F))
#define RESET_DEBOUNCE_TIME(v) (v &= ~0x1F)

#define DEBOUNCE_MODE_BIT (1 << 5)
#define SET_DEBOUNCE_MODE_COUNTER(v) (v |= DEBOUNCE_MODE_BIT)
#define SET_DEBOUNCE_MODE_INTEGRATOR(v) (v &= ~DEBOUNCE_MODE_BIT)
#define DIN_SINK_BIT_START 6

#define ENABLE_COMPARATOR_BIT (1 << 12)
#define ENABLE_COMPARATOR(v) (v |= ENABLE_COMPARATOR_BIT)
#define DISABLE_COMPARATOR(v) (v &= ~ENABLE_COMPARATOR_BIT)

#define INVERT_COMPARATOR_BIT (1 << 13)
#define INVERT_COMPARATOR(v) (v |= INVERT_COMPARATOR_BIT)
#define RESET_INVERT_COMPARATOR(v) (v &= ~INVERT_COMPARATOR_BIT)

#define FILTER_COMPARATOR_BIT (1 << 14)
#define FILTER_COMPARATOR(v) (v |= FILTER_COMPARATOR_BIT)
#define RESET_FILTER_COMPARATOR(v) (v &= ~FILTER_COMPARATOR_BIT)
/* -------------------------------------------------------------------------- */
/* GPO PARALLEL REGISTER                                                      */
/* -------------------------------------------------------------------------- */
#define PARALLEL_CH_A_BIT (1 << 0)
#define PARALLEL_CH_B_BIT (1 << 1)
#define PARALLEL_CH_C_BIT (1 << 2)
#define PARALLEL_CH_D_BIT (1 << 3)

#define PARALLEL_HIGH_CH_A(v) (v |= PARALLEL_CH_A_BIT)
#define PARALLEL_HIGH_CH_B(v) (v |= PARALLEL_CH_B_BIT)
#define PARALLEL_HIGH_CH_C(v) (v |= PARALLEL_CH_C_BIT)
#define PARALLEL_HIGH_CH_D(v) (v |= PARALLEL_CH_D_BIT)

#define PARALLEL_LOW_CH_A(v) (v &= ~PARALLEL_CH_A_BIT)
#define PARALLEL_LOW_CH_B(v) (v &= ~PARALLEL_CH_B_BIT)
#define PARALLEL_LOW_CH_C(v) (v &= ~PARALLEL_CH_C_BIT)
#define PARALLEL_LOW_CH_D(v) (v &= ~PARALLEL_CH_D_BIT)

/* -------------------------------------------------------------------------- */
/* GPO CONFIGURATION                                                          */
/* -------------------------------------------------------------------------- */

#define GPO_SELECT_PULL_DOWN 0x0
#define GPO_SELECT_DRIVE_VIA_CONFIG 0x1
#define GPO_SELECT_DRIVE_VIA_PARALLEL 0x2
#define GPO_SELECT_DEBOUNCE_COMP 0x3
#define GPO_SELECT_HIGH_IMPEDENCE 0x4

#define GPO_SELECT_DATA_BIT (1 << 3)
#define GPO_SELECT_HIGH(x) (x |= GPO_SELECT_DATA_BIT)
#define GPO_SELECT_LOW(x) (x &= ~GPO_SELECT_DATA_BIT)

/* -------------------------------------------------------------------------- */
/* OUTPUT CONFIGURATION                                                       */
/* -------------------------------------------------------------------------- */

#define CURRENT_LIMIT_30mA 0
#define CURRENT_LIMIT_7_5mA 1

#define CLEAR_ENABLE_BIT (1 << 1)
#define ENABLE_CLEAR(v) (v |= CLEAR_ENABLE_BIT)
#define DISABLE_CLEAR(v) (v &= ~CLEAR_ENABLE_BIT)

#define SLEW_ON_MASK (0b01 << 6)
#define SLEW_OFF_MASK (0b10 << 6)
#define ENABLE_SLEW(v) (v |= SLEW_ON_MASK)
#define DISABLE_SLEW(v) (v &= ~SLEW_OFF_MASK)

#define SLEW_STEP_MASK (0b11 << 4)
#define SLEW_STEP_64 (0b00 << 4)
#define SLEW_STEP_120 (0b01 << 4)
#define SLEW_STEP_500 (0b10 << 4)
#define SLEW_STEP_1820 (0b11 << 4)
#define RESET_SLEW_STEP(v) (v &= ~SLEW_STEP_MASK)
#define SET_SLEW_STEP(v, x) (v |= x)

#define SLEW_RATE_MASK (0b11 << 2)
#define SLEW_RATE_4k (0b00 << 2)
#define SLEW_RATE_64k (0b01 << 2)
#define SLEW_RATE_150k (0b10 << 2)
#define SLEW_RATE_240k (0b11 << 2)
#define RESET_SLEW_RATE(v) (v &= ~SLEW_RATE_MASK)
#define SET_SLEW_RATE(v, x) (v |= x)

/* -------------------------------------------------------------------------- */
/* DI THRESHOLD                                                          */
/* -------------------------------------------------------------------------- */

#define TH_SCALE_TO_AVDD (0)
#define TH_FIXED_TO_16V (1)

#define COMPARATOR_TH(v, x) (v |= (x << 1))

/* -------------------------------------------------------------------------- */
/* CONVERSION CONTROL                                                         */
/* -------------------------------------------------------------------------- */

#define DIAG_OFFSET 4

#define ENABLE_ADC_CONVERSION(v, ch) (v |= (1 << ch))
#define DISABLE_ADC_CONVERSION(v, ch) (v &= ~(1 << ch))

#define ENABLE_ADC_DIAG_CONVERSION(v, ch) (v |= (1 << (ch + DIAG_OFFSET)))
#define DISABLE_ADC_DIAG_CONVERSION(v, ch) (v &= ~(1 << (ch + DIAG_OFFSET)))

#define ADC_START_STOP_MASK (0b11 << 8)
#define STOP_CONTINUOUS_MODE_AND_ON (0b00 << 8)
#define START_SINGLE_CONVERSION (0b01 << 8)
#define START_CONTINUOUS_CONVERSION (0b10 << 8)
#define STOP_CONTINUOUS_MODE_AND_OFF (0b11 << 8)
#define RESET_ADC_START_STOP(v) (v &= ~ADC_START_STOP_MASK)
#define SET_ADC_START_STOP(v, x) (v |= x)

#define EN_REJECTION_BIT (1 << 10)
#define ENABLE_DIAG_REJECTION(v) (v |= EN_REJECTION_BIT)
#define DISABLE_DIAG_REJECTION(v) (v &= ~EN_REJECTION_BIT)


#define OPTA_AN_ALERT_VI_ERR_MASK_A (1 << 0)
#define OPTA_AN_ALERT_VI_ERR_MASK_B (1 << 1)
#define OPTA_AN_ALERT_VI_ERR_MASK_C (1 << 2)
#define OPTA_AN_ALERT_VI_ERR_MASK_D (1 << 3)
#define OPTA_AN_HI_TEMP_ERR_MASK (1 << 4)
#define OPTA_AN_CHARGE_PUMP_ERR_MASK (1 << 5)
#define OPTA_AN_ALDO5V_ERR_MASK (1 << 6)
#define OPTA_AN_AVDD_ERR_MASK (1 << 7)
#define OPTA_AN_DVCC_ERR_MASK (1 << 8)
#define OPTA_AN_ALDO1V8_ERR_MASK (1 << 9)
#define OPTA_AN_ADC_CONV_ERR_MASK (1 << 10)
#define OPTA_AN_ADC_SAT_ERR_MAK (1 << 11)
#define OPTA_AN_SPI_SCLK_CNT_ERR_MASK (1 << 12)
#define OPTA_AN_SPI_CRC_ERR_MASK (1 << 13)
#define OPTA_AN_CAL_MEM_ERR_MASK (1 << 14)
// #define DEBUG_ANALOG_PARSE_MESSAGE
#endif
