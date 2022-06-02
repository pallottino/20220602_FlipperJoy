
/*! @date 27 Maggio 2022 Extern Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         marcoangelo.r@alice.it                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#ifndef _EXTERN_H
#define _EXTERN_H

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Files da includere                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Dichiarazione delle variabili globali extern              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#define USB_VOLATILE volatile 

extern struct flagStd01 flagStd_01;
extern struct flagUser01 flagUser_01;

extern volatile uint8_t timeDebounce;
extern volatile uint8_t time2SendUsb;

extern volatile uint16_t counterLedLamp;
extern volatile uint16_t timer0;
extern volatile uint16_t timer1;
extern volatile uint8_t timer2;
extern volatile uint16_t timer3;

extern volatile uint8_t txBufDat[];
extern volatile uint8_t rxBufDat[];
extern volatile USB_HANDLE lastTransmission_handle;

/*! @todo Dati da aggiornare ***
 *
 *  var
 *
 */
// joy_digi
extern volatile uint8_t joyPul[];
extern volatile uint8_t memJoyPul[];
/*! @todo Dati da aggiornare *** End */

#endif
//
// E o f
//
