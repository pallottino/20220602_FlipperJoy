
/*! @date 27 Maggio 2022 User Types Template */

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
//                      Definizione User Types                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#if !defined _USERTYPES_H_
#define _USERTYPES_H_

struct flagStd01 {
    uint8_t bFlTmr0 : 1;
    uint8_t bFlTmr1 : 1;
    uint8_t bFlTmr2 : 1;
    uint8_t bFlTmr3 : 1;
    uint8_t bFlAd : 1;
    uint8_t bFlErr : 1;
    uint8_t bFlDebounce : 1;
    uint8_t bFlBacklite : 1;
};

struct flagUser01 
{
    uint8_t bFlSpeedLamp : 1;
    uint8_t bFlSendUsb : 1;
};
#endif // END IFDEF USERTYPES

//
// E o f
//
