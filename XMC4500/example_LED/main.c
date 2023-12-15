 # include <xmc_common.h>
 #include <system_XMC4500.h>


void initCCU4 ( void ) ;
void connectLED ( void ) ;

int main ( void ) {
    
    initCCU4 () ;
    while (1) ;
    return 0;
}

void initCCU4 () {
   uint8_t mmg[128] = g_chipid;
 /* Release CCU4 instance 0 from reset ( manual 22.6.1 step 2) */
  *(volatile uint32_t *)0x50004414 = 0x1<<2;
  /* Enable clock to CCUs ( manual 22.6.1 step 3) */
    *(volatile uint32_t *)0x50004604 = SCU_CLK_CLKSET_CCUCEN_Msk ;
 /* Enable prescaler in CCU4 instance 0 ( manual 22.6.1 step 4) */
    *(volatile uint32_t *)0x4000C00C = CCU4_GIDLC_SPRB_Msk ;
 /* Configure period and compare in CCU4 instance 0 slice 2 ( manual »
2.6.1 step 6) */
   *(volatile uint32_t *)0x4000C134 = 0xFFFF;
   *(volatile uint32_t *)0x4000C13C = (1 - 0.99) * 0xFFFF;
 /* Request shadow transfer for CCU4 instance 0 slice 2 ( manual »
2.6.1 step 6) */
    *(volatile uint32_t *)0x4000C010 = CCU4_GCSS_S2SE_Msk ;
 /* Connect LED1 to CCU40 . OUT2 */
    connectLED () ;
 /* Enable timer slice 2 in CCU4 instance 0 ( manual 22.6.1 step 8) */
    *(volatile uint32_t *)0x4000C00C = CCU4_GIDLC_CS2I_Msk;
 /* Start slice 2 in CCU4 instance 0 by setting run bit ( manual »
2.6.1 step 9) */
    CCU40_CC42 -> TCSET = CCU4_CC4_TCSET_TRBS_Msk;
 }

 void connectLED () {
 /* Bit mask for alternate function 3 with push - pull output */
 static const uint8_t PP_ALT3 = 0b10011;
 PORT1 -> IOCR0 = ( *(volatile uint32_t *)0x48028110 & ~0x1f<<11 ) | ( PP_ALT3 << 11 );
}