
/*! @date 27 Maggio 2022 10 tasti riga e colonna per Joystick arcade home made */

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
#include "tastiera4Rx4C.h"

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Funzioni definite dall'utente                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

/*! Tasti joystick arcade home made
 *
 *  Colonne su portA/E Output con resistenze di Pull-Up
 *  Righe   su portB   Input  con pulsante in chiusura verso massa
 */
void gestioneTastiera4Rx4C(void)
{
  uint8_t tmpData;

  joyPul[0] = 0;
  joyPul[1] = 0;
  ALLCTRLCOLONNALOW;
  _nop;
  CTRLCOLONNA1HIGH;
  _nop;
  tmpData = (uint8_t) (READALLCTRLRIGA);
  if ((tmpData & CHECKCTRLRIGA1) != 0)
  {
    txBufDat[0] = (uint8_t)MAX_VAL_CONV; // UP
  }
  else if ((tmpData & CHECKCTRLRIGA2) != 0)
  {
    txBufDat[0] = (uint8_t)MIN_VAL_CONV; // DW
  }
  else
  {
    txBufDat[0] = (uint8_t)MID_VAL_CONV; // MID
  }
  if ((tmpData & CHECKCTRLRIGA3) != 0)
  {
    txBufDat[1] = (uint8_t)MIN_VAL_CONV; // SX
  }
  else if ((tmpData & CHECKCTRLRIGA4) != 0)
  {
    txBufDat[1] = (uint8_t)MAX_VAL_CONV; // DX
  }
  else
  {
    txBufDat[1] = (uint8_t)MID_VAL_CONV; // MID
  }
  ALLCTRLCOLONNALOW;
  _nop;
  CTRLCOLONNA2HIGH;
  _nop;
  tmpData = (uint8_t) (READALLCTRLRIGA);
  if ((tmpData & CHECKCTRLRIGA2) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV1;
  }
  if ((tmpData & CHECKCTRLRIGA3) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV2;
  }
  if ((tmpData & CHECKCTRLRIGA4) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV3;
  }
  ALLCTRLCOLONNALOW;
  _nop;
  CTRLCOLONNA3HIGH;
  _nop;
  tmpData = (uint8_t) (READALLCTRLRIGA);
  if ((tmpData & CHECKCTRLRIGA2) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV4;
  }
  if ((tmpData & CHECKCTRLRIGA3) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV5;
  }
  if ((tmpData & CHECKCTRLRIGA4) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV6;
  }
  ALLCTRLCOLONNALOW;
  _nop;
  CTRLCOLONNA4HIGH;
  _nop;
  tmpData = (uint8_t) (READALLCTRLRIGA);
  if ((tmpData & CHECKCTRLRIGA2) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV7;
  }
  if ((tmpData & CHECKCTRLRIGA3) != 0)
  {
    joyPul[0] |= CHECKCTRLDEV8;
  }
  ALLCTRLCOLONNALOW;
  txBufDat[2] = MID_VAL_CONV;
  txBufDat[3] = MID_VAL_CONV;
  txBufDat[4] = MID_VAL_CONV;
  txBufDat[5] = MID_VAL_CONV;
  txBufDat[6] = joyPul[0];
  txBufDat[7] = joyPul[1];
}

/*! Tasti joystick arcade home made 
 *
 *  Colonne su portA/E Input  con resistenze di Pull-Up
 *  Righe   su portB   Output con pulsante normalmente aperto
 * 
 *  Colonne default = High
 *  lettura righe default = High (per pullUp)
 * 
 *  Test pulsanti 
 *  colonna X = 0 
 *  Lettura righe
 *  Riga = 0 con pulsante premuto su colonna X
 * 
 */
void initTastiera4Rx4C (void)
{
  DIRALLCTRLRIGAIN;                        // (DIRCTRLRIGA &= CLEARALLCTRLRIGA)        TRB3 to 0 = OUTPUT
  DIRALLCTRLCOLONNAOUT;                      // (DIRCTRLCOLONNA |= CHECKCTRLCOLONNA1) TRA4
  ALLCTRLCOLONNALOW;
  joyPul[0] = 0;
  joyPul[1] = 0;

  txBufDat[0] = MID_VAL_CONV;
  txBufDat[1] = MID_VAL_CONV;
  txBufDat[2] = MID_VAL_CONV;
  txBufDat[3] = MID_VAL_CONV;
  txBufDat[4] = MID_VAL_CONV;
  txBufDat[5] = MID_VAL_CONV;
  txBufDat[6] = joyPul[0];
  txBufDat[7] = joyPul[1];
}
