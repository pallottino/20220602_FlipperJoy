
/*! @date 27 Maggio 2022 Int Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         marcoangelo.r@alice.it                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Files da includere                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#include "extinc.h"
#include "int.h"

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Dichiarazione delle variabili globali                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Funzione di interrupt                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

/*! \brief Funzione di interrupt */
void __interrupt(high_priority) myHighIsr(void)
{

  #if defined (USB_INTERRUPT)
  {
    if (USBInterruptFlag != 0)
    {
      USBDeviceTasks ();
      USBClearUSBInterrupt ();
    }
  }
  #endif
  if (ADIFISSET)
  {
    CLEARADIF;
  }
  if (TIMER0IFISSET) // Timer 0 overflow ogni 1 mSec.
  {
    TMR0 = timer0;
    CLEARTIMER0IF;
    SETFLTMR0;
    --counterLedLamp;
    if (counterLedLamp == 0)
    {
      if (FLSPEEDLAMPISCLEAR) // Granularita' 0.5 S.
      {
        counterLedLamp = (uint16_t)LEDLAMPRETRY; // LedLampRetry = 500 * 1 mSec. = 0,5 Sec.
      }
      else // Granularita' 0.2 S.
      {
        counterLedLamp = (uint16_t)LEDLAMPSPEEDRETRY; // LedLampRetry = 200 * 1 mSec. = 0,2 Sec.
      }
#if defined(LEDLAMPREADY)
      {
        CTRLLEDLAMPINV;
      }
#endif
    }
    --time2SendUsb;
    if (time2SendUsb == 0)
    {
      time2SendUsb = MAXTIME2SENDUSB;
      SETFLSENDUSB;
    }
    if (timeDebounce)
    {
      --timeDebounce;
      if (timeDebounce == 0)
      {
        SETFLDEBOUNCE;
      }
    }
    if (TIMER1IFISSET)
    {
      CLEARTIMER1IF;
      SETFLTMR1;
    }
    if (TIMER2IFISSET)
    {
      CLEARTIMER2IF;
      CLEARFLTMR2;
    }
    if (TIMER3IFISSET)
    {
      CLEARTIMER3IF;
      SETFLTMR3;
    }
    if (RXUSARTIFISSET) // (PIR1 & 0b00100000) != 0 Bit 5 (RXIF) != 0 Flag interrupt Rx Usart is Set
    {
      CLEARRXUSARTIF;
    }
    if (RBONCHANGEIFISSET) // (INTCON & 0b00000001) != 0 Bit 0 == 1 interrupt on change di porta B e' abilitato
    {
      //
      // A mismatch condition will continue to set this bit. Reading PORTB, and
      // then waiting one additional instruction cycle, will end the mismatch
      // condition and allow the bit to be cleared.
      //
      READPORTRBONCHANGE;
      _nop;
      CLEARRBONCHANGEIF; // INTCON &= 0b11111110 Bit 0 =  0 Reset flag Interrupt on change di porta B
    }
    if (INT0IFISSET) // Int0 usato solo per attivazione modalita Master/Slave con EskyJoyRf
    {
      CLEARINT0IF;
    }
    if (INT1IFISSET)
    {
      CLEARINT1IF;
    }
    if (INT2IFISSET)
    {
      CLEARINT2IF;
    }
  }
}
  //
  // For PIC18 devices the low interrupt vector is found at 00000018h.
  // The following code will branch to the low_interrupt_service_routine
  // function to handle interrupts that occur at the low vector.
  //
  void __interrupt(low_priority) myLowIsr(void)
  {
    #if defined (USB_INTERRUPT)
    {
      if (USBInterruptFlag != 0)
      {
        USBDeviceTasks ();
        USBClearUSBInterrupt ();
      }
    }
    #endif
    READPORTRBONCHANGE; // PORTRBONCHANGE = PORTRBONCHANGE Reset per lettura Int Rb On Change
    _nop;
    CLEARRBONCHANGEIF; // INTCON &= 0b11111110 Bit 0 =  0 Reset flag Interrupt on change di porta B
  }

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //          Inizializzazione del Timer0 per interrupt                         //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  /** \brief Inizializzazione di Timer 0 Overflow
   *
   *   Inizializzazione di Timer 0
   *
   *   \return Timer 0 Interrupt disabilitato
   *   \return Timer 0 spento
   */
  void initTimer0(void)
  {

    PRIORITYINTOFF;  // RCON  &= 0b01000000  Bit 7 (IPEN) = 0 disabilita interrupt priority (PIC16FXX compatibility Mode)
    TIMER016BIT;     // T0CON &= 0b10111111  Bit 6 = 0 abilita contatore a 16 bit
    TIMER0CLOCKXTAL; // T0CON &= 0b11011111  Bit 5 = 0 Timer 0 Clock Source internal (CLK0 = XTAL / 4)
    TIMER0EDGEFAL;   // T0CON |= 0b00010000  Bit 4 = 1 Timer0 increment on Falling edge
    PRETIMER0OFF;    // T0CON |= 0b00001000 Bit 3 = 1 prescaler bypass  (Timer 0 Prescaler 1:1)
    TIMER0PRERESET;  // T0CON &= 0b11111000 Bit 2-0 = 000 = 1:2 prescale value RESET (Clear bit 2-0 Timer0 Prescaler)
    TIMER0OFF;       // T0CON &= 0b01111111  Bit 7 = 0 disabilita Timer 0
    TIMER0INTOFF;    // INTCON &= 0b11011111 Bit 5 = 0 disabilita Timer 0 overflow interrupt
    TIMER0CLOCKXTAL; // T0CON &= 0b11011111  Bit 5 = 0 Timer 0 Clock Source internal (CLK0 = XTAL / 4)
    TMR0 = timer0;
    CLKPLLOFF;     // OSCTUNE &= 0b10111111 Bit 6 (PLLEN)  = 0   Disable Clock Pll
    CLEARTIMER0IF; // INTCON &= 0b11111011 Bit 2 = 0 Clear flag overflow interrupt TIMER0
    CLEARFLTMR0;   // Flag di servizio
  }

  void enaTimer0(uint16_t time)
  {

    DI;
    TIMER0OFF; // T0CON &= 0b01111111 Bit 7 = 0 stop Timer 0
    TMR0 = time;
    TIMER0INTON;   // INTCON |= 0b00100000 Bit 5 =  1 abilita Timer 0 overflow interrupt
    CLEARTIMER0IF; // INTCON &= 0b11111011 Bit 2 =  0 Clear flag overflow interrupt TIMER0
    CLEARFLTMR0;   // Flag di servizio
    TIMER0ON; // T0CON &= 0b10000000 Bit 7 = 1 Timer0 ON
    EI;
  }

  void disTimer0(void)
  {

    DI;
    TIMER0INTOFF; // INTCON  &= 0b11011111 Bit 5 =  0 disabilita Timer 0 overflow interrupt
#if !defined(_JEPI_16F6XX)
    TIMER0OFF;
#endif
    CLEARTIMER0IF; // Attivazione Timer0 setta FlagTimer0If
    CLEARFLTMR0;   // Flag di servizio
    EI;
  }

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //                  Inizializzazione di Timer 1                               //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  void initTimer1(void)
  {

    // T1CON:   TIMER1 CONTROL REGISTER
    // bit 7    RD16:   16-bit Read/Write Mode Enable bit
    //                  1 = Enables register Read/Write of Timer1 in one 16-bit op
    //                  0 = Enables register Read/Write of Timer1 in two 8-bit op
    // bit 6    T1RUN: Timer1 System Clock Status bit
    //                  1 = Device clock is derived from Timer1 oscillator
    //                  0 = Device clock is derived from another source
    // bit 5-4  T1CKPS1:T1CKPS0: Timer1 Input Clock Prescaler Select bits
    //                  11 = 1:8 Prescaler value
    //                  10 = 1:4 Prescaler value
    //                  01 = 1:2 Prescaler value
    //                  00 = 1:1 Prescaler value
    // bit 3    T1OSCEN: Timer1 Oscillator Enable bit
    //                  1 = Timer1 Oscillator is enabled
    //                  0 = Timer1 Oscillator is shut-off
    //                  The oscillator inverter and feedback resistor are turned off
    //                  to eliminate power drain.
    // bit 2    T1SYNC: Timer1 External Clock Input Synchronization Select bit
    //                  When TMR1CS = 1:
    //                  1 = Do not synchronize external clock input
    //                  0 = Synchronize external clock input
    //                  When TMR1CS = 0:
    //                  This bit is ignored. Timer1 uses the internal clock when
    //                  TMR1CS = 0.
    // bit 1    TMR1CS: Timer1 Clock Source Select bit
    //                  1 = External clock from pin RC0/T1OSO/T13CKI (on the rising edge)
    //                  0 = Internal clock (FOSC/4)
    // bit 0    TMR1ON: Timer1 On bit
    //                  1 = Enables Timer1
    //                  0 = Stops Timer1
    T1CON = 0;
    TIMER1PRERESET;       // T1CON &= 0b11001111 Bit 5-4 = 00 prescale value 1:1 (Reset prescaler value)
    TIMER1CLOCKSOURCEINT; // T1CON &= 0b11111101 Bit   1 (TMR1CS)  = 0 Timer1Clock = Internal clock (FOSC/4)
    TIMER1EXTOSCOFF;      // T1CON &= 0b11110111 Bit   3 = 0 Timer1 External Osc Disabled (Settare pin In prima di abilitare oscillatore)
    TIMER1SYNCOFF;        // T1CON |= 0b00000100 Bit   2 (T1SYNC)  = 1 External clock not sync (only when TMR1CS = 1)
    // TIMER1SYNCON; // T1CON &= 0b11111011 Bit   2 (T1SYNC)  = 0 External clock     sync (only when TMR1CS = 1)
    CLEARTIMER1IF; // Clear Flag Timer1 Overflow
    TIMER1OFF;     // Spegni Timer 1
    TIMER1INTOFF;  // Disabilita interrupt timer 1
    TIMER1_8BIT;   // T1CON &= 0b01111111 Bit 7 (RD16)  = 0 Enables Timer1 2 *  8 bit register Read/Write (Default per Counter 16 bit linguaggio C)
    TMR1 = timer1;
    CLEARTIMER1IF;
    CLEARFLTMR1;
  }

  void enaTimer1(uint16_t time)
  {

    DI;
    TIMER1OFF; // T1CON &= 0b11111110 Bit 0 = 0 per stop Timer 1 clock
    TMR1 = time;
    TIMER1ON; // T1CON &= 0b00000001 Bit 0 = 1 Timer1 ON
    TIMER1INTON;   // PIE1 |= 0b00000001 Bit 0 =  1 abilita Timer 1 overflow interrupt
    CLEARTIMER1IF; // PIR1 &= 0b11111110 Bit 0 =  0 Clear flag overflow interrupt TIMER1
    CLEARFLTMR1;   // Flag di servizio
    EI;
  }

  void disTimer1(void)
  {

    DI;
    CLEARTIMER1IF;
    TIMER1OFF;
    TIMER1INTOFF;
    CLEARFLTMR1; // Flag di servizio
    EI;
  }

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //                  Inizializzazione di Timer 2                               //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  void initTimer2(void)
  {
    // T2CON: TIMER2 CONTROL REGISTER (ADDRESS 12h)
    // bit 7:   Unimplemented: Read as '0'
    // bit 6-3: TOUTPS3:TOUTPS0: Timer2 Output Postscale Select bits
    //          0000 = 1:1 Postscale
    //          0001 = 1:2 Postscale
    //          0010 = 1:3 Postscale
    //          1111 = 1:16 Postscale
    // bit 2:   TMR2ON: Timer2 On bit
    //          1 = Timer2 is on
    //          0 = Timer2 is off
    // bit 1-0: T2CKPS1:T2CKPS0: Timer2 Clock Prescaler Select bits
    //          00 = Prescaler is 1
    //          01 = Prescaler is 4
    //          1x = Prescaler is 16
    // T2CON = 0b00000001 Attiva Timer 2 Postscaler 4:1 e Prescaler = 1:1

    // PIE1:    PERIPHERAL INTERRUPT ENABLE REGISTER 1
    // bit 7    PSPIE(1): Parallel Slave Port Read/Write Interrupt Enable bit
    //          1 = Enables the PSP read/write interrupt
    //          0 = Disables the PSP read/write interrupt
    // bit 6    ADIE: A/D Converter Interrupt Enable bit
    //          1 = Enables the A/D interrupt
    //          0 = Disables the A/D interrupt
    // bit 5    RCIE: USART Receive Interrupt Enable bit
    //          1 = Enables the USART receive interrupt
    //          0 = Disables the USART receive interrupt
    // bit 4    TXIE: USART Transmit Interrupt Enable bit
    //          1 = Enables the USART transmit interrupt
    //          0 = Disables the USART transmit interrupt
    // bit 3    SSPIE: Master Synchronous Serial Port Interrupt Enable bit
    //          1 = Enables the MSSP interrupt
    //          0 = Disables the MSSP interrupt
    // bit 2    CCP1IE: CCP1 Interrupt Enable bit
    //          1 = Enables the CCP1 interrupt
    //          0 = Disables the CCP1 interrupt
    // bit 1    TMR2IE: TMR2 to PR2 Match Interrupt Enable bit
    //          1 = Enables the TMR2 to PR2 match interrupt
    //          0 = Disables the TMR2 to PR2 match interrupt
    // bit 0    TMR1IE: TMR1 Overflow Interrupt Enable bit
    //          1 = Enables the TMR1 overflow interrupt
    //          0 = Disables the TMR1 overflow interrupt
    TMR2 = 0;        // Reset di Timer 2
    TIMER2PRERESET;  // T2CON &= 0b11111000 prescaler value = 1: 1 (Reset prescaler)
    TIMER2POSTRESET; // T2CON &= 0b10000111 postcaler value = 1: 1 (Reset postcaler)
    TIMER2OFF;       // T2CON &= 0b11111011 Bit 2 (TMR2ON) == 0 disabilita Timer 2
    TIMER2INTOFF;    // Disabilita interrupt timer 2
    TMR2 = timer2;
    CLEARTIMER2IF;
    CLEARFLTMR2;
  }

  void enaTimer2(uint8_t time)
  {

    DI;
    TIMER2OFF;       // T2CON &= 0b11111011 Bit 2 (TMR2ON) == 0 disabilita Timer 2
    TMR2 = time;
    TIMER2ON;  // T2CON |= 0b00000100 Bit 2 (TMR2ON) == 1 abilita Timer 2
    TIMER2INTON;   // PIE1 |= 0b00000010 Bit 1 =  1 abilita Timer 2 overflow interrupt
    CLEARTIMER2IF; // PIR1 &= 0b11111101 Bit 1 =  0 Clear flag overflow interrupt TIMER2
    CLEARFLTMR2;   // Flag di servizio
    EI;
  }

  void disTimer2(void)
  {

    DI;
    CLEARTIMER2IF;
    TIMER2OFF;       // T2CON &= 0b11111011 Bit 2 (TMR2ON) == 0 disabilita Timer 2
    TIMER2INTOFF;
    EI;
  }

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //                  Inizializzazione di Timer 3                               //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  void initTimer3(void)
  {
#ifndef MICRO18PIN

    // T3CON:   TIMER3 CONTROL REGISTER
    // bit 7    RD16:   16-bit Read/Write Mode Enable bit
    //                  1 = Enables register Read/Write of Timer3 in one 16-bit op
    //                  0 = Enables register Read/Write of Timer3 in two 8-bit op
    // bit 6,3  T3CCP2:T3CCP1: Timer3 and Timer1 to CCPx Enable bits
    //                  1x = Timer3 is the capture/compare clock source for the CCP modules
    //                  01 = Timer3 is the capture/compare clock source for CCP2;
    //                        Timer1 is the capture/compare clock source for CCP1
    //                  00 = Timer1 is the capture/compare clock source for the CCP modules
    // bit 5-4  T3CKPS1:T1CKPS0: Timer3 Input Clock Prescaler Select bits
    //                  11 = 1:8 Prescaler value
    //                  10 = 1:4 Prescaler value
    //                  01 = 1:2 Prescaler value
    //                  00 = 1:1 Prescaler value
    // bit 2    T3SYNC: Timer1 External Clock Input Synchronization control bit
    //                      (Not usable if the device clock comes from Timer1/Timer3.)
    //                  When TMR3CS = 1:
    //                  1 = Do not synchronize external clock input
    //                  0 = Synchronize external clock input
    //                  When TMR3CS = 0:
    //                  This bit is ignored. Timer3 uses the internal clock when
    //                      TMR3CS = 0.
    // bit 1    TMR3CS: Timer1 Clock Source Select bit
    //                  1 = External clock input from Timer1 oscillator or T13CKI (on the rising edge after the first
    //                      falling edge)
    //                  0 = Internal clock (FOSC/4)
    // bit 0    TMR3ON: Timer3 On bit
    //                  1 = Enables Timer3
    //                  0 = Stops Timer3

    // PIE2:  PERIPHERAL INTERRUPT ENABLE REGISTER 2
    // bit 7  OSCFIE: Oscillator Fail Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 6  CMIE: Comparator Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 5  Unimplemented: Read as '0'
    // bit 4  EEIE: Data EEPROM/Flash Write Operation Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 3  BCLIE: Bus Collision Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 2  HLVDIE: High/Low-Voltage Detect Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 1  TMR3IE: TMR3 Overflow Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled
    // bit 0  CCP2IE: CCP2 Interrupt Enable bit
    //        1 = Enabled
    //        0 = Disabled

    // PIR2:  PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 2
    // bit 7  OSCFIF: Oscillator Fail Interrupt Flag bit
    //        1 = Device oscillator failed, clock input has changed to INTOSC (must be cleared in software)
    //        0 = Device clock operating
    // bit 6  CMIF: Comparator Interrupt Flag bit
    //        1 = Comparator input has changed (must be cleared in software)
    //        0 = Comparator input has not changed
    // bit 5  Unimplemented: Read as '0'
    // bit 4  EEIF: Data EEPROM/Flash Write Operation Interrupt Flag bit
    //        1 = The write operation is complete (must be cleared in software)
    //        0 = The write operation is not complete or has not been started
    // bit 3  BCLIF: Bus Collision Interrupt Flag bit
    //        1 = A bus collision occurred (must be cleared in software)
    //        0 = No bus collision occurred
    // bit 2  HLVDIF: High/Low-Voltage Detect Interrupt Flag bit
    //        1 = A high/low-voltage condition occurred (direction determined by
    //            VDIRMAG bit, HLVDCON<7>)
    //        0 = A high/low-voltage condition has not occurred
    // bit 1  TMR3IF: TMR3 Overflow Interrupt Flag bit
    //        1 = TMR3 register overflowed (must be cleared in software)
    //        0 = TMR3 register did not overflow
    // bit 0  CCP2IF: CCPx Interrupt Flag bit
    //        Capture mode:
    //        1 = A TMR1 register capture occurred (must be cleared in software)
    //        0 = No TMR1 register capture occurred
    //        Compare mode:
    //        1 = A TMR1 register compare match occurred (must be cleared in software)
    //        0 = No TMR1 register compare match occurred
    //        PWM mode:
    //        Unused in this mode

    T3CON = 0;
    TIMER3_08BIT;         // T3CON &= 0b01111111 Bit 7 = 0 Enables register Read/Write of Timer1 in two 8-bit op
    TIMER3PRERESET;       // T3CON &= 0b11001111 Bit 5-4 = 00 prescaler value 1:1 (Reset prescaler)
    TIMER3CLOCKSOURCEINT; // T3CON &= 0b11111101 Bit   1 (TMR1CS)  = 0 Timer1Clock = Internal clock (FOSC/4)
    TIMER3SYNCON;         // T3CON &= 0b11111011 Bit 2 = 0 External clock sync
    CLEARTIMER3IF;        // Clear flag timer3 overflow
    TIMER3OFF;            // Prima di aggiornare il valore spengere il timer
    TIMER3OFF; // T3CON &= 0b11111110 Bit 0 = 0 disabilita Timer 3
    CLEARFLTMR3;
#endif
  }

  void enaTimer3(uint16_t time)
  {

    DI;
    TIMER3OFF; // T3CON &= 0b11111110 Bit 0 = 0 disabilita Timer 3
    TMR3 = time;
    TIMER3ON; // T3CON |= 0b00000001 Bit 0 = 1 abilita Timer 3
    TIMER3INTON;   // PIE2 &= 0b11111101 Bit 1 =  0 abilita Timer 3 Int Overflov
    CLEARTIMER3IF; // PIR2 &= 0b11111110 Bit 1 =  0 Clear flag overflow interrupt TIMER1
    CLEARFLTMR3;   // Flag di servizio
    EI;
  }

  void disTimer3(void)
  {
    DI;
    CLEARTIMER3IF;
    TIMER3OFF; // T3CON &= 0b11111110 Bit 0 = 0 disabilita Timer 3
    TIMER3INTOFF;
    EI;
  }

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  // CCPxCON: STANDARD CCPx CONTROL REGISTER                                    //
  // bit 7-6  Unimplemented: Read as '0' (1)                                    //
  // bit 5-4  DCxB1:DCxB0: PWM Duty Cycle Bit 1 and Bit 0 for CCPx Module       //
  //          Capture mode: Unused.                                             //
  //          Compare mode: Unused.                                             //
  //          PWM mode:                                                         //
  //          These bits are the two LSbs (bit 1 and bit 0) of the 10-bit PWM   //
  //           duty cycle. The eight MSbs of the duty cycle are found in CCPR1L.//
  // bit 3-0  CCPxM3:CCPxM0: CCPx Module Mode Select bits                       //
  //          0000 = Capture/Compare/PWM disabled (resets CCPx module)          //
  //          0001 = Reserved                                                   //
  //          0010 = Compare mode: toggle output on match (CCPxIF bit is set)   //
  //          0011 = Reserved                                                   //
  //          0100 = Capture mode: every falling edge                           //
  //          0101 = Capture mode: every rising edge                            //
  //          0110 = Capture mode: every 4th rising edge                        //
  //          0111 = Capture mode: every 16th rising edge                       //
  //          1000 = Compare mode: initialize CCPx pin low; on compare match,   //
  //                  force CCPx pin high (CCPxIF bit is set)                   //
  //          1001 = Compare mode: initialize CCPx pin high; on compare match,  //
  //                  force CCPx pin low (CCPxIF bit is set)                    //
  //          1010 = Compare mode: generate software interrupt on compare match //
  //                  (CCPxIF bit is set, CCPx pin reflects I/O state)          //
  //          1011 = Compare mode: trigger special event, reset timer, start    //
  //                  A/D conversion on CCP2 match (CCPxIF bit is set)          //
  //          11xx = PWM mode                                                   //
  //                                                                            //
  // The assignment of a particular timer to a module is determined by the Timer//
  // to CCP enable bits in the T3CON register (Register 14-1).                  //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  // PIE1:    PERIPHERAL INTERRUPT ENABLE REGISTER 1
  // bit 7    PSPIE(1): Parallel Slave Port Read/Write Interrupt Enable bit
  //          1 = Enables the PSP read/write interrupt
  //          0 = Disables the PSP read/write interrupt
  // bit 6    ADIE: A/D Converter Interrupt Enable bit
  //          1 = Enables the A/D interrupt
  //          0 = Disables the A/D interrupt
  // bit 5    RCIE: USART Receive Interrupt Enable bit
  //          1 = Enables the USART receive interrupt
  //          0 = Disables the USART receive interrupt
  // bit 4    TXIE: USART Transmit Interrupt Enable bit
  //          1 = Enables the USART transmit interrupt
  //          0 = Disables the USART transmit interrupt
  // bit 3    SSPIE: Master Synchronous Serial Port Interrupt Enable bit
  //          1 = Enables the MSSP interrupt
  //          0 = Disables the MSSP interrupt
  // bit 2    CCP1IE: CCP1 Interrupt Enable bit
  //          1 = Enables the CCP1 interrupt
  //          0 = Disables the CCP1 interrupt
  // bit 1    TMR2IE: TMR2 to PR2 Match Interrupt Enable bit
  //          1 = Enables the TMR2 to PR2 match interrupt
  //          0 = Disables the TMR2 to PR2 match interrupt
  // bit 0    TMR1IE: TMR1 Overflow Interrupt Enable bit
  //          1 = Enables the TMR1 overflow interrupt
  //          0 = Disables the TMR1 overflow interrupt

  /*
  void initCcp1(void)
  {
    CCP1COMPAREON;
    SETCCP1FALLINGEDGE;
    CCP2COMPAREOFF;
    SETCCP2FALLINGEDGE;
    TRISC |= 0b00000100;        // Ccp1 Input
    TRISC &= 0b11111101;        // Disabilita CCP2
    CCP1INTOFF;                 // Disabilita interrupt di Ccp1
    CLEARCCP1IF;                // Clear flag int Ccp1
  }
   */

  void initCcp1Capture(void)
  {

    DIRCCP1CTRLOUT;     // TRISC &= 0b11111011 TRC2 = OUTPUT
    RESETCCP1;          // CCP1CON &= 0b11110000 CCP1 RESET (ECCP MODULE OFF)
    CCP1CAPFALLINGEDGE; // CCP1CON &= 0b11110100
    CCP1INTOFF;         // Disabilita interrupt di Ccp1
    CLEARCCP1IF;        // Clear flag int Ccp1
  }

  void enaCcp1Capture(uint8_t fallRis)
  {

    CCP1INTOFF; // Disabilita interrupt di Ccp1
    RESETCCP1;  // CCP1CON &= 0b11110000 CCP1 RESET (ECCP MODULE OFF)
    switch (fallRis)
    {
    case 1:
      CCP1CAPFALLINGEDGE; // CCP1CON &= 0b11110100
      break;
    case 0:
      CCP1CAP1RISINGEDGE; // CCP1CON |= 0b00000101
      break;
    }
    TIMER3CCPSOURCE; // Set Timer3 Source counter Ccp mode
    DIRCCP1CTRLOUT;  // Setta Output  (abilita)    TrisC2 (Ccp1)
    CLEARCCP1IF;     // Clear flag int Ccp1
  }

  void disCcp1(void)
  {

    CLEARCCP1IF;    // Clear flag int Ccp1
    CCP1INTOFF;     // Disabilita interrupt di Ccp1
    RESETCCP1;      // CCP1CON &= 0b11110000 CCP1 RESET (ECCP MODULE OFF)
    DIRCCP1CTRLOUT; // TRISC &= 0b11111011         Tris C 2 = 0
  }

  void enaCcp2(uint8_t fallRis)
  {

    CCP2INTOFF; // Disabilita interrupt di Ccp2
    RESETCCP2;  // CCP2CON &= 0b11110000 CCP2 RESET (ECCP MODULE OFF)
    switch (fallRis)
    {
    case 1:
      CCP2CAPFALLINGEDGE; // CCP2CON &= 0b11110100
      break;
    case 0:
      CCP2CAP1RISINGEDGE; // CCP2CON |= 0b00000101
      break;
    }
    TIMER3CCPSOURCE; // Set Timer3 Source counter Ccp mode
    DIRCCP2CTRLOUT;  // Setta Output  (abilita)    TrisC1 (Ccp2)
    CLEARCCP2IF;     // Clear flag int Ccp2
  }

  void disCcp2(void)
  {

    CLEARCCP2IF;    // Clear flag int Ccp2
    CCP2INTOFF;     // Disabilita interrupt di Ccp2
    RESETCCP2;      // CCP2CON &= 0b11110000 CCP2 RESET (ECCP MODULE OFF)
    DIRCCP2CTRLOUT; // DIRCCP2BIT = INPUT (TRISC1 = 1)
  }

// IPR1:  PERIPHERAL INTERRUPT PRIORITY REGISTER 1
// bit 7  PSPIP: Parallel Slave Port Read/Write Interrupt Priority bit(1)
//        1 = High priority
//        0 = Low priority
//        Note 1: This bit is unimplemented on 28-pin devices and will read as '0'
// bit 6  ADIP: A/D Converter Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 5  RCIP: EUSART Receive Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 4  TXIP: EUSART Transmit Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 3  SSPIP: Master Synchronous Serial Port Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 2  CCP1IP: CCP1 Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 1  TMR2IP: TMR2 to PR2 Match Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
// bit 0  TMR1IP: TMR1 Overflow Interrupt Priority bit
//        1 = High priority
//        0 = Low priority
#define INT0EDGERIS INTCON2 |= 0b01000000 // Bit 6 = 1 Rising   edge Int 0 (0 -> 1)
#define INT0EDGEFAL INTCON2 &= 0b10111111 // Bit 6 = 0 Falling  edge Int 0 (1 -> 0) (RESET)
#define INT1EDGERIS INTCON2 |= 0b00100000 // Bit 5 = 1 Rising   edge Int 1 (0 -> 1)
#define INT1EDGEFAL INTCON2 &= 0b11011111 // Bit 5 = 0 Falling  edge Int 1 (1 -> 0) (RESET)

  void enaInt0Falling(void)
  {
    DI;
    INT0EDGEFAL; // INTCON2 &= 0b10111111    Bit 6 = 0 Falling  edge Int 0 (1 -> 0) (RESET)
    _nop;
    CLEARINT0IF; // Clear flag Int 0
    _nop;
    INT0ON; // Abilita interrupt da INT0 (RB0)
    EI;
  }

  void enaInt0Rising(void)
  {
    DI;
    INT0EDGEFAL; // INTCON2 &= 0b10111111    Bit 6 = 0 Falling  edge Int 0 (1 -> 0) (RESET)
    _nop;
    INT0EDGERIS; // INTCON2 |= 0b01000000       // Bit 6 = 1 Rising   edge Int 0 (0 -> 1)
    _nop;
    CLEARINT0IF; // INTCON &= 0b11111101 Bit 1 =  0 Int 0 flag reset
    _nop;
    INT0ON; // Abilita interrupt da INT0 (RB0)
    EI;
  }

  void disInt0(void)
  {

    INT0EDGEFAL; // INTCON2 &= 0b10111111    Bit 6 = 0 Falling  edge Int 0 (1 -> 0) (RESET)
    CLEARINT0IF; // Clear flag Int 0
    INT0OFF;     // Disabilita interrupt da INT0 (RB0)
  }

  void enaInt1Falling(void)
  {
    DI;
    INT1EDGEFAL; // INTCON2 &= 0b11011111 Bit 5 = 0 Falling  edge Int 1 (1 -> 0) (RESET)
    _nop;
    CLEARINT1IF; // Clear flag Int 1
    _nop;
    INT1ON; // Abilita interrupt da INT1 (RB1)
    EI;
  }

  void enaInt1Rising(void)
  {
    DI;
    INT1EDGEFAL; // INTCON2 &= 0b11011111 Bit 5 = 0 Falling  edge Int 1 (1 -> 0) (RESET)
    INT1EDGERIS; // INTCON2 |= 0b00100000 Bit 5 = 1 Rising   edge Int 1 (0 -> 1)
    _nop;
    CLEARINT1IF; // Clear flag Int 0
    INT1ON;      // Abilita interrupt da INT1 (RB1)
    EI;
  }

  void disInt1(void)
  {

    INT1EDGEFAL; // INTCON2 &= 0b11011111 Bit 5 = 0 Falling  edge Int 1 (1 -> 0) (RESET)
    _nop;
    CLEARINT1IF; // Clear flag Int 1
    INT1OFF;     // Disabilita interrupt da INT1 (RB1)
  }

  void enaInt2Falling(void)
  {
    DI;
    INT2EDGEFAL; // Int2 scatta quando pin passa 1 -> 0
    CLEARINT2IF; // Clear flag Int 2
    INT2ON;      // Abilita interrupt da INT2 (RB2)
    EI;
  }

  void enaInt2Rising(void)
  {
    DI;
    INT2EDGERIS; // Int1 scatta quando pin passa 0 -> 1
    CLEARINT2IF; // Clear flag Int 2
    INT2ON;      // Abilita interrupt da INT2 (RB2)
    EI;
  }

  void disInt2(void)
  {
    CLEARINT2IF; // Clear flag Int 2
    INT2OFF;     // Disabilita interrupt da INT2 (RB2)
  }

  void enaIntRbOnChange(void)
  {

    DI;
    TRISB &= 0b00001111; // Rb4-7 = Output
    _nop;
    TRISB |= 0b01110000; // Rb 6-4 Input per IntRbOnChange Pedaliera
    READPORTRBONCHANGE;  // PORTB = PORTB Reset per lettura Int Rb On Change
    _nop;
    CLEARRBONCHANGEIF; // INTCON &= 0b11111110 bit 0 =  0 Interrupt on change di porta B flag reset
    INTRBONCHANGEON;   // Abilita interrupt on change su PortB
    EI;
  }

  void disIntRbOnChange(void)
  {

    DI;
    INTRBONCHANGEOFF;   // Disabilita interrupt on change su PortB
    READPORTRBONCHANGE; // PORTB = PORTB Reset per lettura Int Rb On Change
    _nop;
    CLEARRBONCHANGEIF; // INTCON &= 0b11111110 bit 0 =  0 Interrupt on change di porta B flag reset
    EI;
  }

  void initWdt(void)
  {

#if defined WDTREADY // Risveglio da Sleep = 0.625 Sec.
#if defined SLEEPREADY
    CCP1CON = 0; // Disabilita periferica Ccp
    CCP1INTOFF;  // Disabilita interrupt di Ccp1
    CLEARCCP1IF; // Clear flag int Ccp1
    TXEN = 0;    // Pin Tx = I/O
    SPEN = 0;    // Disabilita Usart - Abilita pin usart per I/O
    OPTION_REG = 0;
    PRETIMER0ON;    // T0CON |= 0b11110111 Bit 3 = 0 prescaler ON
    TIMER0PRERESET; // T0CON &= 0b11111000 Bit 2-0 = 000 = 1:2 prescale value RESET (Clear bit 2-0 Timer0 Prescaler)
    TIMER0PRE256;   // Prescaler 1:256 = 1:128 per Wdt = 18 mS. * 128 = 2.3 S.
    TRISA = 0;
    PORTA = 0;
    TRISB = 0b10110000;
    PORTB = 0b10110011; // Led Off = 1
#endif
#endif
  }

  // PIR1:  PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 1
  // bit 7  PSPIF: Parallel Slave Port Read/Write Interrupt Flag bit(1)
  //        1 = A read or a write operation has taken place (must be cleared in software)
  //        0 = No read or write has occurred
  // bit 6  ADIF: A/D Converter Interrupt Flag bit
  //        1 = An A/D conversion completed (must be cleared in software)
  //        0 = The A/D conversion is not complete
  // bit 5  RCIF: EUSART Receive Interrupt Flag bit
  //        1 = The EUSART receive buffer, RCREG, is full (cleared when RCREG is read)
  //        0 = The EUSART receive buffer is empty
  // bit 4  TXIF: EUSART Transmit Interrupt Flag bit
  //        1 = The EUSART transmit buffer, TXREG, is empty (cleared when TXREG is written)
  //        0 = The EUSART transmit buffer is full
  // bit 3  SSPIF: Master Synchronous Serial Port Interrupt Flag bit
  //        1 = The transmission/reception is complete (must be cleared in software)
  //        0 = Waiting to transmit/receive
  // bit 2  CCP1IF: CCP1 Interrupt Flag bit
  //        Capture mode:
  //          1 = A TMR1 register capture occurred (must be cleared in software)
  //          0 = No TMR1 register capture occurred
  //        Compare mode:
  //          1 = A TMR1 register compare match occurred (must be cleared in software)
  //          0 = No TMR1 register compare match occurred
  //        PWM mode:
  //          Unused in this mode.
  //          bit 1 TMR2IF: TMR2 to PR2 Match Interrupt Flag bit
  //          1 = TMR2 to PR2 match occurred (must be cleared in software)
  //          0 = No TMR2 to PR2 match occurred
  // bit 0  TMR1IF: TMR1 Overflow Interrupt Flag bit
  //          1 = TMR1 register overflowed (must be cleared in software)
  //          0 = TMR1 register did not overflow

  //
  ////////////////////////////////////////////////////////////////////////////////
  //                                                                            //
  //                  Fine file                                                 //
  //                                                                            //
  ////////////////////////////////////////////////////////////////////////////////
  //

  //
  // E o f
  //
