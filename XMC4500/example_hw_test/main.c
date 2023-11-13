/* HW test example project
 * Echos via USB virtual serial port what is sent to it and allows to turn the LEDs on and off via
 * buttons so all parts of a Relax Lite Kit that are used in the programming assignments can be
 * tested
 *****/

#include "VirtualSerial.h"
#include <xmc_gpio.h>

/* Clock configuration */
XMC_SCU_CLOCK_CONFIG_t clock_config = {
    .syspll_config.p_div = 2,
    .syspll_config.n_div = 80,
    .syspll_config.k_div = 4,
    .syspll_config.mode = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL,
    .syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP,
    .enable_oschp = true,
    .calibration_mode = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY,
    .fsys_clksrc = XMC_SCU_CLOCK_SYSCLKSRC_PLL,
    .fsys_clkdiv = 1,
    .fcpu_clkdiv = 1,
    .fccu_clkdiv = 1,
    .fperipheral_clkdiv = 1
};


void SystemCoreClockSetup(void) {
    /* Setup settings for USB clock */
    XMC_SCU_CLOCK_Init(&clock_config);

    XMC_SCU_CLOCK_EnableUsbPll();
    XMC_SCU_CLOCK_StartUsbPll(2, 64);
    XMC_SCU_CLOCK_SetUsbClockDivider(4);
    XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);
    XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

    SystemCoreClockUpdate();
}

/* GPIO configuration */
#define GPIO_LED2     XMC_GPIO_PORT1,  0
#define GPIO_LED1     XMC_GPIO_PORT1,  1
#define GPIO_BUTTON1  XMC_GPIO_PORT1, 14
#define GPIO_BUTTON2  XMC_GPIO_PORT1, 15

const XMC_GPIO_CONFIG_t out_config = \
      {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};
const XMC_GPIO_CONFIG_t in_config = \
      {.mode=XMC_GPIO_MODE_INPUT_TRISTATE,\
       .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
       .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};

/**
 * Main program entry point. This routine configures the hardware required by
 * the application, then enters a loop to run the application tasks in sequence.
 */
int main(void) {
  uint16_t Bytes = 0;

  XMC_GPIO_Init(GPIO_LED1,     &out_config);
  XMC_GPIO_Init(GPIO_LED2,     &out_config);
  XMC_GPIO_Init(GPIO_BUTTON1,  &in_config);
  XMC_GPIO_Init(GPIO_BUTTON2,  &in_config);
  USB_Init();

  while (1) {
    /* Check if data received */
    Bytes = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface);

    while(Bytes > 0) {
      /* Send data back to the host */
      CDC_Device_SendByte(&VirtualSerial_CDC_Interface,
                          CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface));
      --Bytes;
    }

    if(XMC_GPIO_GetInput(GPIO_BUTTON1) == 0) {
      XMC_GPIO_SetOutputHigh(GPIO_LED1);
    } else {
      XMC_GPIO_SetOutputLow(GPIO_LED1);
    }
    if(XMC_GPIO_GetInput(GPIO_BUTTON2) == 0) {
      XMC_GPIO_SetOutputHigh(GPIO_LED2);
    } else {
      XMC_GPIO_SetOutputLow(GPIO_LED2);
    }

    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
  }
  return 0;
}
