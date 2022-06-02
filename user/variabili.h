
/*! @date 27 Maggio 2022 Variables Template */

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
//                  Inclusione files                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Dichiarazione delle variabili globali                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

struct flagStd01 flagStd_01;
struct flagUser01 flagUser_01;

volatile uint8_t timeDebounce;
volatile uint8_t time2SendUsb;
volatile uint16_t counterLedLamp;
volatile uint16_t timer0;
volatile uint16_t timer1;
volatile uint8_t timer2;
volatile uint16_t timer3;

volatile uint8_t txBufDat[MAXTXBUFDAT] JOYSTICK_TXDATA_ADDRESS;
volatile uint8_t rxBufDat[MAXRXBUFDAT] JOYSTICK_RXDATA_ADDRESS;
volatile USB_HANDLE lastTransmission_handle;

/*! @todo Dati da aggiornare ***
 *
 *  var
 *
 */
//joy_digi
/*! @todo Dati da aggiornare *** End */
volatile uint8_t joyPul[2];
volatile uint8_t memJoyPul[2];

//
// E o f
//
