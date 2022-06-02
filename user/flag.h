
/*! @date 27 Maggio 2022 Flags Template */

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
//                  Inizializzazione CPU e Release                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

// Controllo flag flagStd_01
#define SETFLTMR0               flagStd_01.bFlTmr0 = 1
#define CLEARFLTMR0             flagStd_01.bFlTmr0 = 0
#define FLTMR0ISCLEAR           flagStd_01.bFlTmr0 == 0
#define FLTMR0ISSET             flagStd_01.bFlTmr0 != 0

#define SETFLTMR1               flagStd_01.bFlTmr1 = 1
#define CLEARFLTMR1             flagStd_01.bFlTmr1 = 0
#define FLTMR1ISCLEAR           flagStd_01.bFlTmr1 == 0
#define FLTMR1ISSET             flagStd_01.bFlTmr1 != 0

#define SETFLTMR2               flagStd_01.bFlTmr2 = 1
#define CLEARFLTMR2             flagStd_01.bFlTmr2 = 0
#define FLTMR2ISCLEAR           flagStd_01.bFlTmr2 == 0
#define FLTMR2ISSET             flagStd_01.bFlTmr2 != 0

#define SETFLTMR3               flagStd_01.bFlTmr3 = 1
#define CLEARFLTMR3             flagStd_01.bFlTmr3 = 0
#define FLTMR3ISCLEAR           flagStd_01.bFlTmr3 == 0
#define FLTMR3ISSET             flagStd_01.bFlTmr3 != 0

#define SETFLAD                 flagStd_01.bFlAd = 1
#define CLEARFLAD               flagStd_01.bFlAd = 0
#define FLADISSET               flagStd_01.bFlAd != 0
#define FLADISCLEAR             flagStd_01.bFlAd == 0

#define SETFLERR                flagStd_01.bFlErr = 1
#define CLEARFLERR              flagStd_01.bFlErr = 0
#define FLERRISSET              flagStd_01.bFlErr != 0
#define FLERRISCLEAR            flagStd_01.bFlErr == 0

#define SETFLDEBOUNCE           flagStd_01.bFlDebounce = 1
#define CLEARFLDEBOUNCE         flagStd_01.bFlDebounce = 0
#define FLDEBOUNCEISCLEAR       flagStd_01.bFlDebounce == 0
#define FLDEBOUNCEISSET         flagStd_01.bFlDebounce != 0

#define SETFLBACKLITE           flagStd_01.bFlBacklite = 1
#define CLEARFLBACKLITE         flagStd_01.bFlBacklite = 0
#define FLBACKLITEISSET         flagStd_01.bFlBacklite != 0
#define FLBACKLITEISCLEAR       flagStd_01.bFlBacklite == 0

// Controllo Flag User_01
#define SETFLSPEEDLAMP          flagUser_01.bFlSpeedLamp = 1
#define CLEARFLSPEEDLAMP        flagUser_01.bFlSpeedLamp = 0
#define FLSPEEDLAMPISSET        flagUser_01.bFlSpeedLamp != 0
#define FLSPEEDLAMPISCLEAR      flagUser_01.bFlSpeedLamp == 0

#define SETFLSENDUSB            flagUser_01.bFlSendUsb = 1
#define CLEARFLSENDUSB          flagUser_01.bFlSendUsb = 0
#define FLSENDUSBISSET          flagUser_01.bFlSendUsb != 0
#define FLSENDUSBISCLEAR        flagUser_01.bFlSendUsb == 0

//
// E o f
//
