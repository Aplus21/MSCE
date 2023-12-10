#include "config.h"

char secretCode [] = "I CAN MORSE";



int main(void)
{
    XMC_GPIO_CONFIG_t config;

    config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
    config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
    config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM;

    XMC_GPIO_Init(LED1, &config);

    DelayInit();
    delayMs(DOT_TIME);
    while(1)
    {
        textToMorse(secretCode); //.. / -.-. .- -. / -- --- .-. ... .
        // Space between Transmission
        delayMs(REPEAT_TIME);
    }
    
}