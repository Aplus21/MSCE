#include <xmc_gpio.h>

int main(void) {
  const XMC_GPIO_CONFIG_t LED_config = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
         .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
         .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

  XMC_GPIO_Init(XMC_GPIO_PORT1, 0, &LED_config);

  while(1) {
    for(int32_t waiter=(1<<20); waiter >= 0; waiter--);
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
  }
  return 0;
}
