/*
 * Copyright (c) 2022, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */

/* MBED TARGET LIST: ZEST_CORE_FMLR-72 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
} ALTx;

typedef enum {
    PA_0       = 0x00,
    PA_1       = 0x01,
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_5       = 0x05,
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_14_ALT0 = PA_14 | ALT0, // same pin used for alternate HW
    PA_15      = 0x0F,
    PB_0       = 0x10,
    PB_1       = 0x11,
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_4       = 0x14,
    PB_4_ALT0  = PB_4  | ALT0, // same pin used for alternate HW
    PB_5       = 0x15,
    PB_5_ALT0  = PB_5  | ALT0, // same pin used for alternate HW
    PB_6       = 0x16,
    PB_7       = 0x17,
    PB_8       = 0x18,
    PB_9       = 0x19,
    PB_10      = 0x1A,
    PB_11      = 0x1B,
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_14      = 0x1E,
    PB_15      = 0x1F,
    PC_0       = 0x20,
    PC_1       = 0x21,
    PC_2       = 0x22,
    PC_4       = 0x24,
    PC_5       = 0x25,
    PC_6       = 0x26,
    PC_6_ALT0  = PC_6  | ALT0, // same pin used for alternate HW
    PC_7       = 0x27,
    PC_7_ALT0  = PC_7  | ALT0, // same pin used for alternate HW
    PC_8       = 0x28,
    PC_9       = 0x29,
    PC_10      = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0, // same pin used for alternate HW
    PC_11      = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0, // same pin used for alternate HW
    PC_12      = 0x2C,
    PC_13      = 0x2D,
    PC_14      = 0x2E,
    PC_15      = 0x2F,
    PD_2       = 0x32,
    PH_0       = 0x70,
    PH_1       = 0x71,

    /**** ADC internal channels ****/

    ADC_TEMP = 0xF0, // Internal pin virtual value
    ADC_VREF = 0xF1, // Internal pin virtual value
    ADC_VBAT = 0xF2, // Internal pin virtual value

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PA_2,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PA_3,
#endif

    /**** Zest connector signal namings (P1) ****/
    P1_I2C_SCL      = PB_6,
    P1_I2C_SDA      = PB_9,
    P1_UART_RX      = PA_10,
    P1_UART_TX      = PA_9,
    P1_SPI_MOSI     = PB_5,
    P1_SPI_MISO     = PB_4,
    P1_SPI_SCK      = PB_3,
    P1_SPI_CS       = PB_2,
    P1_PWM1         = PC_6,
    P1_PWM2         = PA_0,
    P1_PWM3         = PC_7,
    P1_ADC1         = PB_0,
    P1_ADC2         = PB_1,
    P1_ADC3         = PC_1,
    P1_DIO1         = PA_5,
    P1_DIO2         = PA_11,
    P1_DIO3         = PA_12,
    P1_DIO4         = PC_13,
    P1_DIO5         = PA_8,
    P1_DIO6         = PD_2,
    P1_DIO7         = PB_7,
    P1_DIO8         = PC_12,
    P1_DIO9         = PA_15,
    P1_DIO12        = PC_11,
    P1_DIO18        = PC_10,

    LORA_MOSI       = PB_15,
    LORA_MISO       = PB_14,
    LORA_SCK        = PB_10,
    LORA_CS         = PA_4,
    LORA_RESET      = PC_2,
    LORA_DIO0       = PA_1,
    LORA_DIO1       = PA_6,
    LORA_DIO2       = PA_7,
    LORA_DIO3       = PC_4,
    LORA_DIO4       = PC_5,
    LORA_DIO5       = PB_11,
    LORA_ANTSW_PWR  = PB_12,

    /**** SPI flash pins ****/
    FLASH_SPI_MOSI  = PB_5,
    FLASH_SPI_MISO  = PB_4,
    FLASH_SPI_CLK   = PB_3,
    FLASH_SPI_CS    = PB_13,

    /**** OSCILLATOR pins ****/
    RCC_OSC32_IN = PC_14,
    RCC_OSC32_OUT = PC_15,

    /**** DEBUG pins ****/
    SYS_SWCLK = PA_14,
    SYS_SWDIO = PA_13,
    SYS_WKUP1 = PA_0,
    SYS_WKUP2 = PC_13,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

// Standardized LED and button names
#define LED1     PH_0 // 6TRON Board
#define LED2     PB_8 // FMLR
#define BUTTON1  PH_1

#ifdef __cplusplus
}
#endif

#endif
