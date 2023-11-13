/*
 * Copyright (C) 2014 Infineon Technologies AG. All rights reserved.
 *
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon's microcontrollers.
 * This file can be freely distributed within development tools that are
 * supporting such microcontrollers.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/**
 * @file
 * @date 04 Dec, 2014
 * @version 1.0.0
 *
 * @brief XMC4500 Relax kit GPIO toggle example
 *
 * GPIO toggle example flashes the leds of the board with a periodic rate.
 * LED1 is connected to P1.1
 * LED2 is connected to P1.0
 *
 * History <br>
 *
 * Version 1.0.0 Initial <br>
 *
 */

#include <xmc_gpio.h>
#include <XMC4500.h>
#include <xmc4_gpio.h>
#include <ctype.h>

#define LED1 P1_1

#define DOT_TIME 100
#define DASH_TIME (DOT_TIME * 3)
#define LETTER_TIME (DOT_TIME * 3)
#define SPACE_TIME (DOT_TIME * 7)
#define REPEAT_TIME (DOT_TIME * 500)
#define TICKS_PER_SECOND 1000

static volatile bool tick_1000ms_elapsed = false;
static volatile uint32_t ms_ticks = 0;
char secretCode [] = "I CAN MORSE";

static __IO uint32_t msTicks;

// Morse code mapping
const char *morseCode[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---",
    "...--", "....-", ".....", "-....", "--...", "---..", "----.", "/", " "
};

// SysTick_Handler function will be called every 1 ms
void SysTick_Handler()
{
    if (msTicks != 0)
    {
        msTicks--;
    }
}

void DelayInit()
{
    // Update SystemCoreClock value
    SystemCoreClockUpdate();
    // Configure the SysTick timer to overflow every 1 s
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);
}

void DelayMs(uint32_t ms)
{
    // Reload ms value
    msTicks = ms;
    // Wait until msTick reach zero
    while (msTicks){

    }
}

void dot(){
    XMC_GPIO_SetOutputHigh(LED1);
    DelayMs(DOT_TIME);
    XMC_GPIO_SetOutputLow(LED1);
}

void dash(){
    XMC_GPIO_SetOutputHigh(LED1);
    DelayMs(DASH_TIME);
    XMC_GPIO_SetOutputLow(LED1);
}

void textToMorse(const char *text) {
    for (int i = 0; i < strlen(text); i++) {
        char c = tolower(text[i]);
        if (c >= 'a' && c <= 'z') {
            const char *morse = morseCode[c - 'a'];
            for (int j = 0; morse[j] != '\0'; j++) {
                if (morse[j] == '.') {
                    // Dot: 100ms
                    dot();
                } else if (morse[j] == '-') {
                    // Dash: 300ms
                    dash();
                }
                // Space between Morse characters (1 dot)
                DelayMs(DOT_TIME);
            }
        } else if (c == ' ') {
            // Space between words (7 dots)
            DelayMs(SPACE_TIME);
        }
        // Space between Morse letters (3 dots)
        DelayMs(LETTER_TIME);
    }
    // Space between Transmission
    DelayMs(REPEAT_TIME);
}

int main(void)
{
    XMC_GPIO_CONFIG_t config;

    config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
    config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
    config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM;

    XMC_GPIO_Init(LED1, &config);

    DelayInit();
    while(1)
    {
        textToMorse(secretCode); //.. / -.-. .- -. / -- --- .-. ... .
    }
    
}