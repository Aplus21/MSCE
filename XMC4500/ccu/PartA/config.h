#include <xmc_gpio.h>
#include <XMC4500.h>
#include <xmc4_gpio.h>
#include <ctype.h>

#define LED1 P1_1

#define DOT_TIME 100
#define DASH_TIME (DOT_TIME * 3)
#define LETTER_TIME (DOT_TIME * 2) //Space between letters - 1 Space between Morse characters
#define SPACE_TIME (DOT_TIME * 4) //Space between words - 3 from Letter space
#define REPEAT_TIME (DOT_TIME * 47) //Space between transmission 50 - 3 Letter space
#define TICKS_PER_SECOND 1000

void DelayInit();
void delayMs(uint32_t ms);
void SysTick_Handler();
void dot();
void dash();
void textToMorse(const char *text);