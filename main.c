
/*! @date 27 Maggio 2022 Start Main Template */

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
//                            Files da includere                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#include "extinc.h"
#include "variabili.h"
#include "main.h"
#include "user\usertypes.h"

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Dichiarazione delle variabili globali                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//

struct flagStd01 flagStd_01;
struct flagUser01 flagUser_01;

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Dichiarazione delle variabili extern                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

extern void USBDeviceInit(void);
extern void APP_DeviceJoystickInitialize(void);
#if defined(USB_INTERRUPT)
  extern void USBDeviceAttach(void);
  #endif

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Inizio codice                                             //
//                  Funzioni definite dall'utente                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
// ATTENZIONE: Configurare PLL DIV per 48 MHZ
//

void main(void)
{

  _nop;
  _nop;
  initPort();
  initRtc48(); // Attiva oscillatore interno USB 48 MHz., tick istruzioni a 12MHz. e setta Timer0_1MS
  PHEINTON; // Abilita interrupt anche da periferiche che usano low ISR
#if defined DEBUGIOREADY
  initDebug();
  ALLDEBUGLOW;
#endif
  EI; // Abilita General Interrupt
#if defined LEDLAMPREADY
  initLedLamp(); // Posizionale per TRISB
#endif
  USBDeviceInit(); // usb_device.c.  Initializes USB module SFRs and firmware
  lastTransmission_handle = 0;
  //lastReception_handle = 0;
#if defined(USB_INTERRUPT)
  USBDeviceAttach();
  #endif
  APP_DeviceJoystickInitialize();
#if !defined USB_INTERRUPT
  time2SendUsb = MAXTIME2SENDUSB;
  CLEARFLSENDUSB;
  SETFLSPEEDLAMP;
  while (USBGetDeviceState() < CONFIGURED_STATE)
  {
    USBDeviceTasks();
    USBClearUSBInterrupt ();
  }
#endif
  CLEARFLSPEEDLAMP;
  initTastiera4Rx4C();
  PRIORITYINTOFF; // RCON &= 0b01111111 Bit 7 (IPEN) = 0 dsabilita interrupt priority
  while (1)
    {
      if (FLSENDUSBISSET)
      {
        CLEARFLSENDUSB;
        gestioneTastiera4Rx4C();
        //gestioneJoyDigi ();
        APP_DeviceJoystickTasks ();
        USBDeviceTasks ();
        USBClearUSBInterrupt ();
      }
    }
}

//
// E o f
//
