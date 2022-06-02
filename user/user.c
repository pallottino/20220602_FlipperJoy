
/*! @date 24 Aprile 2022 User Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        Junior Electronics Pisa                             //
//                                                                            //
//                             Ufficio  Tecnico                               //
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

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Inizio codice                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//          Funzioni di inizializzazione standard                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#if defined LEDLAMPREADY

void initLedLamp(void)
{
  DIRCTRLLEDLAMPOUT;
  _nop;
  CTRLLEDLAMPON;
  counterLedLamp = LEDLAMPRETRY;
}
#endif

void initAllFlag(void)
{

  CLEARFLTMR0; /*! \brief Flag Std_01 */
  CLEARFLTMR1;
  CLEARFLTMR2;
  CLEARFLTMR3;
  CLEARFLAD;
  CLEARFLERR;
  CLEARFLDEBOUNCE;
  CLEARFLBACKLITE;
  CLEARFLSPEEDLAMP; /*! \brief Flag User_01 */
  CLEARFLSENDUSB;
}

void myReset(void)
{

#if defined(_PIC_H_)
  asm("goto 0x0000");
#else
  Reset();
#endif
}

void initPort(void)
{
  TRISA = 0xff;
  TRISB = 0xff;
  TRISC = 0xff;
  #if defined MICRO40PIN
    TRISD = 0xff;
    TRISE = 0b11101111; // RE4 = 0 per disabilitare Psp Mode
    #endif
  initPrescaler();
  ADCON1 |= 0b00001111; // Porta A = digital I/O
  CVRCON = 0;
  initTimer0();         // Default Timer0Off TimerInt0Off
  initTimer1();         // Default Timer1Off TimerInt1Off
  initTimer2();         // Default Timer2Off TimerInt2Off
  initTimer3();         // Default Timer3Off TimerInt3Off
  enaTimer0(timer0);    // Timer 0 Int Ovf abilitato a 1 ms.
  disTimer1(); // Timer 1 Int Ovf abilitato a 50 ms.
  disTimer2(); // Timer 2 Int Ovf abilitato a 0 ms.
  disTimer3(); // Timer 3 Int Ovf abilitato a 0 ms.
  disIntRbOnChange();   // Disabilita IntRbOnChange (Default)
  initAllFlag();
  PULLUPOFF;      // Disabilitare PullUp per Rx dati Abilitare per R/W Eeprom (Pin Rb1 Data In)
}

//
// E o f
//
