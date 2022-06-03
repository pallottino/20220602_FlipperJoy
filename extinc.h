
/*! @date 27 Maggio 2022 External Include Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         marcoangelo.r@alice.it                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#pragma warning disable 752
#pragma warning disable 520

/*! @todo *** Valori da aggiornare */
#define FLIPPERJOYREADY
//#define DEBUGICD3READY            // Disabilita funzioni che interessano RB6 (CONNETTERE JUMPER AL TERMINE)

//#define DEBUGUSBREADY
//#define DEBUG_SETUP_PKT_READY   // Abilita invio dati PacketSetup via Rs232
#define LEDLAMPREADY // Led lampeggiante
//#define DEBUGIOREADY            // Linee di debug attive

//#define LCDREADY
#if defined LCDREADY
#define LCDSERREADY
//#define LCDSTDREADY
#define LCD20X4READY
#endif
/*! @todo *** end */

#include <xc.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*! Ad uso del programma vsCode */
#include "C:\Programmi\Microchip\xc8\v2.36\pic\include\proc\pic18f4550.h"
//#include "C:\Programmi\Microchip\xc8\v2.36\pic\include\proc\pic18f4550.inc"

#include "HardwareProfile.h" // (usb_config.h viene incluso da HardwareProfile.h)
#include "app_device_joystick.h"
#include "fixed_address_memory.h"
#include "system.h"
#include "usb.h"
#include "usb_ch9.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb_device_hid.h"
#include "usb_hal_pic18.h"

#if defined(_HTC_H_)
#define _nop NOP()
#else
#define _nop Nop()
#endif

#define CLEARRS232IF RCREG = RCREG // Reset Flag Interrupt Usart (RCIE bit 5 = 0)
#define USB_VOLATILE volatile
#define _CRUNTIME // Liberare la Ram Gpr2 per cBufDat

#define NONE 0
#define FLAGON 1
#define FLAGOFF 0
#define SPACE ' '
#define PRINTNEWPAGE 0x0c
#define ENDOFLINE 0x00
#define LINEFEED 0x0A
#define CLEARPAGE 0x0C
#define CARRYRETURN 0x0D
#define ENDOFSTRING 0x00
#define ENDOFBUFFER 0xffff
#define STARTDISPLCD 0

#define NUMBYTE2SEND 8
#define NUMASSI2SEND 5
#define NUMPUL2SEND 2 // 2 * 8 bit per 16 pulsanti
#define MAXTIME2SENDUSB 10
#define MAXTIME2DEBOUNCE 10
#define TIMETOSEND 10
#define TIMER_0MS 0           // Setting per timer non in uso
#define LEDLAMPRETRY 500      // 500 iterazioni per 1 mSec. LedLampInv = 500 mSec.
#define LEDLAMPSPEEDRETRY 150 // 150 iterazioni per 1 mSec. LedLampInv = 200 mSec. Lamp Speed per taratura
#define USB_HANDLE void*

#if defined LEDLAMPREADY
#define DIRLEDLAMP TRISB
#define LEDLAMPBIT PORTB
#define LEDLAMPLAT LATB

#define CLEARLEDLAMP 0b01111111 // RB7
#define CHECKLEDLAMP 0b10000000 // RB7

#define DIRCTRLLEDLAMPIN DIRLEDLAMP |= CHECKLEDLAMP  // RB7
#define DIRCTRLLEDLAMPOUT DIRLEDLAMP &= CLEARLEDLAMP // RB7

#define CTRLLEDLAMPON LEDLAMPLAT |= CHECKLEDLAMP  // RB7
#define CTRLLEDLAMPOFF LEDLAMPLAT &= CLEARLEDLAMP // RB7
#define CTRLLEDLAMPINV LEDLAMPLAT ^= CHECKLEDLAMP // RB7
#endif

#include "extern.h"
#include "int\int.h"
#include "flag.h"
#include "user.h"
#include "funzioni.h"

#ifndef _USERTYPES_H_
#include "user\usertypes.h"
#endif

/*! @fixme Definire anche in extinc.h */
#define USB_POLLING
//#define USB_INTERRUPT

//*! @todo *** Valori da aggiornare */
//#include "include_usb.h"
#include "tastiera4Rx4C\tastiera4Rx4C.h"

#define TIMER_0MS 0 // Setting per timer non in uso
#define MAXRXBUFDAT 8
#define MAXTXBUFDAT 8
#define MAXASSE  6
#define _XTAL_FREQ (double)12000000 // USB clock = 48 MHz. Tick = 1/4 = 12000000

#define HID_INTF_ID 0x00
#define HID_EP 1
#define HID_INT_OUT_EP_SIZE 64
#define HID_INT_IN_EP_SIZE 64
//#define HID_INT_OUT_EP_SIZE     8
//#define HID_INT_IN_EP_SIZE      8
#define HID_NUM_OF_DSC 1
#define HID_RPT01_SIZE 51
#define JEPI_VID 0x0008 // Vendor ID, see usb_config.h
#define JEPI_PID 0x0008 // Product ID, see usb_config.h
#define JEPI_BCD 0x0002 // Bcd Device Number
#define USB_NUM_STRING_DESCRIPTORS 6

/*! @todo *** Valori da aggiornare */
#define MIN_VAL_CONV      -128 // Variare valori da 0 / 255 per lettura [] desiderata
#define MID_VAL_CONV      0
#define MAX_VAL_CONV      127
#define CHECKCTRLDEV1                    0x0001 
#define CHECKCTRLDEV2                    0x0002 
#define CHECKCTRLDEV3                    0x0004 
#define CHECKCTRLDEV4                    0x0008 
#define CHECKCTRLDEV5                    0x0010 
#define CHECKCTRLDEV6                    0x0020 
#define CHECKCTRLDEV7                    0x0040 
#define CHECKCTRLDEV8                    0x0080
#include "tastiera4Rx4C\tastiera4Rx4C.h"
/*! @todo *** End */

//
// EOF
//
