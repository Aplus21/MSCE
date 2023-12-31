#include "config.h"


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

void delayMs(uint32_t ms)
{
    // Reload ms value
    msTicks = ms;
    // Wait until msTick reach zero
    while (msTicks){

    }
}

void dot(){
    XMC_GPIO_SetOutputHigh(LED1);
    delayMs(DOT_TIME);
    XMC_GPIO_SetOutputLow(LED1);
}

void dash(){
    XMC_GPIO_SetOutputHigh(LED1);
    delayMs(DASH_TIME);
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
                delayMs(DOT_TIME);
            }
        } else if (c == ' ') {
            // Space between words (7 dots)
            delayMs(SPACE_TIME);
            continue;
        }
        // Space between Morse letters (3 dots)
        delayMs(LETTER_TIME);
    }
}
