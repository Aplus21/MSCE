#include <xmc_gpio.h>
#include <XMC4500.h>
#include <xmc4_gpio.h>
#include <ctype.h>
#include <stdio.h>

#define LED1 P1_1
#define LED2 P1_0
#define GPIO_BUTTON1  XMC_GPIO_PORT1, 14
#define GPIO_BUTTON2  XMC_GPIO_PORT1, 15

#define DOT_TIME 100
#define DASH_TIME (DOT_TIME * 3)
#define LETTER_TIME (DOT_TIME * 3)
#define SPACE_TIME (DOT_TIME * 7)
#define REPEAT_TIME (DOT_TIME * 500)
#define TICKS_PER_SECOND 1000

const XMC_GPIO_CONFIG_t out_config = \
      {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};
const XMC_GPIO_CONFIG_t in_config = \
      {.mode=XMC_GPIO_MODE_INPUT_TRISTATE,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

static volatile bool startCount = false;
static volatile uint32_t msCounter = 0;
char secretCode [] = "I CAN MORSE";
int gapTime = 0;
char gapText[32];

static __IO uint32_t msTicks;

// Morse code mapping
const char *morseCode[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
    "....", "..", ".---", "-.-", ".-..", "--", "-.",
    "---", ".--.", "--.-", ".-.", "...", "-", "..-",
    "...-", ".--", "-..-", "-.--", "--..", "-----",
    ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----.", "/", " "
};

// SysTick_Handler function will be called every 1 ms
void SysTick_Handler()
{
    msCounter++;
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

void delay_ms(uint32_t ms)
{
    // Reload ms value
    msTicks = ms;
    // Wait until msTick reach zero
    while (msTicks){

    }
}

void dot(){
    XMC_GPIO_SetOutputHigh(LED1);
    delay_ms(DOT_TIME);
    XMC_GPIO_SetOutputLow(LED1);
}

void dash(){
    XMC_GPIO_SetOutputHigh(LED1);
    delay_ms(DASH_TIME);
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
                delay_ms(DOT_TIME);
            }
        }
        else if (c >= '0' && c <= '9') {
            const char *morse = morseCode[(c - '0') + 26];
            for (int j = 0; morse[j] != '\0'; j++) {
                if (morse[j] == '.') {
                    // Dot: 100ms
                    dot();
                } else if (morse[j] == '-') {
                    // Dash: 300ms
                    dash();
                }
                // Space between Morse characters (1 dot)
                delay_ms(DOT_TIME);
            }
        }
        else if (c == ' ') {
            // Space between words (7 dots)
            delay_ms(SPACE_TIME);
        }
        // Space between Morse letters (3 dots)
        delay_ms(LETTER_TIME);
    }
}

int main(void)
{   
    XMC_GPIO_Init(LED1, &out_config);
    XMC_GPIO_Init(LED2,     &out_config);
    XMC_GPIO_Init(GPIO_BUTTON1,  &in_config);
    XMC_GPIO_Init(GPIO_BUTTON2,  &in_config);

    DelayInit();
    
    while(1)
    {
        if(XMC_GPIO_GetInput(GPIO_BUTTON1) == 0) {
            gapTime = msCounter - gapTime;
            textToMorse(secretCode); //.. / -.-. .- -. / -- --- .-. ... .
        } else {
            XMC_GPIO_SetOutputLow(LED2);
        }
        if(XMC_GPIO_GetInput(GPIO_BUTTON2) == 0) {
            sprintf(gapText, "%d", gapTime);
            textToMorse(gapText);
        } else {
            XMC_GPIO_SetOutputLow(LED2);
        }
    }
}