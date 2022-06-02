
/*! @date 27 Maggio 2022 funzioni varie Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         marcoangelo.r@alice.it //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Files da includere //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#include "extinc.h"

void initRtc48 (void) // Set modalita' lavoro impostata da registro config
{                // Oscillatore xTal 4 MHz. + Pll Usb = 48 MHz.

  // xTal = 48 MHz
  // Tick = 1 / (48^6 / 4 / 1 (Prescaler)) = 83333^-12
  // Settare Timer0 per Int 1 ms.
  // 1^-3 / 83333^-12 = 12000
#undef _XTAL_FREQ
#undef TIMER0_1MS
//#define TIMER0_1MS 17535   // TIMER0_1MS con XTal = 48 MHz = 65535 - 48000
#define _XTAL_FREQ (double)12000000 // USB clock = 48 MHz. Tick = 1/4 = 12000000
#define TIMER0_1MS 53535 // TIMER0_1MS con XTal =  4 MHz + Pll Usb = 65535 - 12000
  timer0 = TIMER0_1MS;
  PRIMARYOSC; // OSCCON  &= 0b11111100  Bit 1-0 = 0-0 Primary oscillator (Reset)
}

void initPrescaler (void)
{
  PRETIMER0OFF;   // T0CON |= 0b00001000 Bit 3 = 1 prescaler bypass  (Timer 0 Prescaler 1:1)
  TIMER0PRERESET; // T0CON &= 0b11111000 Bit 2-0 = 000 = 1:2 prescale value RESET (Clear bit 2-0 Timer0 Prescaler)
  TIMER1PRERESET; // T1CON &= 0b11001111 Bit 5-4 = 00 prescale value 1:1 (Reset prescaler value)
  TIMER2PRERESET; // T2CON &= 0b11111100 Bit 1-0 = 00   prescaler value  1: 1 (Reset prescaler)
  TIMER2POSTRESET; // T2CON &= 0b10000111 Bit 6/3 = 0000 postscaler value 1: 1 (Reset postscaler)
  TIMER3PRERESET;  // T3CON &= 0b11001111 Bit 5-4 = 00 prescaler value 1:1 (Reset prescaler)
  timer0 = TIMER_0MS;
  timer1 = TIMER_0MS;
  timer2 = TIMER_0MS;
  timer3 = TIMER_0MS;
}
