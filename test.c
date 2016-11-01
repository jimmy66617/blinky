#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer16.h"
#include "clkconfig.h"
#include "gpio.h"

#define TEST_TIMER_NUM  0  /* 0 or 1 for 16-bit timers only */

extern volatile uint32_t timer16_0_counter;
extern volatile uint32_t timer16_1_counter;

/* Main Program */

int main (void) {
  uint32_t interval;

 SystemInit();

  /* Config CLKOUT, mostly used for debugging. */
  CLKOUT_Setup( CLKOUTCLK_SRC_MAIN_CLK );
  LPC_IOCON->PIO0_1 &= ~0x07; 
  LPC_IOCON->PIO0_1 |= 0x01;  /* CLK OUT */

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

  /* TEST_TIMER_NUM is either 0 or 1 for 16-bit timer 0 or 1. */
  interval = SystemAHBFrequency/1000 - 1;
  if ( interval > 0xFFFF )
  {
 interval = 0xFFFF;
  }
  init_timer16(TEST_TIMER_NUM, interval);
  enable_timer16(TEST_TIMER_NUM);

  /* Set port 0_7 to output */
   GPIOSetDir( 2, 7, 1 );

  while (1)                                /* Loop forever */
  {
#if TEST_TIMER_NUM
 /* I/O configuration and LED setting pending. */
 if ( (timer16_1_counter > 0) && (timer16_1_counter <= 500) )
 {
   GPIOSetValue( 2, 7, 0 );
 }
 if ( (timer16_1_counter > 500) && (timer16_1_counter <= 1000) )
 {
   GPIOSetValue( 2, 7, 1 );
 }
 else if ( timer16_1_counter > 1000 )
 {
   timer16_1_counter = 0;
 }
#else
 /* I/O configuration and LED setting pending. */
 if ( (timer16_0_counter > 0) && (timer16_0_counter <= 500) )
 {
   GPIOSetValue( 2, 7, 0 );
 }
 if ( (timer16_0_counter > 500) && (timer16_0_counter <= 1000) )
 {
   GPIOSetValue( 2, 7, 1 );
 }
 else if ( timer16_0_counter > 1000 )
 {
   timer16_0_counter = 0;
 }
#endif
  }
}