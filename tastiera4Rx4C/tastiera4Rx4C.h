
/*! @date 6 Maggio 2020 Tastiera 4 Righe x 4 Colonne */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        Junior Electronics Pisa                             //
//                                                                            //
//                      via Ordine di S. Stefano 128                          //
//                      56013         Marina di Pisa                          //
//                      Tel.050-35251  Fax.050-35251                          //
//                                                                            //
//                             Ufficio  Tecnico                               //
//                      Per. Ind.  Angelo  Raglianti                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Prototipi delle funzioni                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

void gestioneTastiera4Rx4C (void);
void lasciaTasto (void);
void initTastiera4Rx4C (void);
void gestioneJoyDigi (void);

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Definizione special function registers                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
/*! Joystick digitale 4 assi 
 *  K dei diodi portato a massa tramite RB0-3 OUT Riga1-4
 *  
 *  colonna X HIGH->LOW con riga X = LOW + pulsante PRE
 */
#define DIRCTRLASSI TRISA
#define CTRLASSIBIT PORTA
#define CTRLASSILAT LATA
/*! Tasti joystick arcade home made 
 *
 *  Colonne INPUT  PORTA (Pull-Up 10 K)
 *  Righe   OUTPUT PORTB (K diodi verso GND)
 *  Pulsante in chiusura verso massa 
 *  colonna X HIGH->LOW con riga X = LOW + pulsante PRE
 * 
 */
#define DIRCTRLRIGA             TRISB
#define CTRLRIGABIT             PORTB
#define CTRLRIGALAT             LATB

#define CLEARALLCTRLRIGA            0b11110000                            // RB3 to RB0
#define CHECKALLCTRLRIGA            0b00001111                            // RB3 to RB0

#define CLEARCTRLRIGA1              0b11111110                            // RB0
#define CHECKCTRLRIGA1              0b00000001                            // RB0

#define CLEARCTRLRIGA2              0b11111101                            // RB1
#define CHECKCTRLRIGA2              0b00000010                            // RB1

#define CLEARCTRLRIGA3              0b11111011                            // RB2
#define CHECKCTRLRIGA3              0b00000100                            // RB2

#define CLEARCTRLRIGA4              0b11110111                            // RB3
#define CHECKCTRLRIGA4              0b00001000                            // RB3

#define CLEARCTRLCOLJOYPUL           0b11011111                            // RB5
#define CHECKCTRLCOLJOYPUL           0b00100000                            // RB5

/*! Brief CTRLRIGA OUT */
#define DIRALLCTRLRIGAIN            DIRCTRLRIGA |= CHECKALLCTRLRIGA        // TRB3 to 0

#define READALLCTRLRIGA             CTRLRIGABIT & CHECKALLCTRLRIGA         // RB3 to 0

/*! Brief Colonna IN */
#define DIRCTRLCOLONNA       TRISA
#define CTRLCOLONNABIT       PORTA
#define CTRLCOLONNALAT       LATA

#define CLEARALLCTRLCOLONNA         0b11110000                            // R3-0
#define CHECKALLCTRLCOLONNA         0b00001111                            // R3-0

#define CLEARCTRLCOLONNA1           0b11111110                            // RA0
#define CHECKCTRLCOLONNA1           0b00000001                            // RA0

#define CLEARCTRLCOLONNA2           0b11111101                            // RA1
#define CHECKCTRLCOLONNA2           0b00000010                            // RA1
 
#define CLEARCTRLCOLONNA3           0b11111011                            // RA2
#define CHECKCTRLCOLONNA3           0b00000100                            // RA2

#define CLEARCTRLCOLONNA4           0b11110111                            // RA3
#define CHECKCTRLCOLONNA4           0b00001000                            // RA3


#define DIRALLCTRLCOLONNAIN         DIRCTRLCOLONNA |= CHECKALLCTRLCOLONNA  // TRA3-0
#define DIRALLCTRLCOLONNAOUT        DIRCTRLCOLONNA &= CLEARALLCTRLCOLONNA  // TRA3-0

#define DIRCTRLCOLONNA1IN           DIRCTRLCOLONNA |= CHECKCTRLCOLONNA1  // TRA0
#define DIRCTRLCOLONNA1OUT          DIRCTRLCOLONNA &= CLEARCTRLCOLONNA1  // TRA0

#define DIRCTRLCOLONNA2IN           DIRCTRLCOLONNA |= CHECKCTRLCOLONNA2  // TRA1
#define DIRCTRLCOLONNA2OUT          DIRCTRLCOLONNA &= CLEARCTRLCOLONNA2  // TRA1

#define DIRCTRLCOLONNA3IN           DIRCTRLCOLONNA |= CHECKCTRLCOLONNA3  // TRA2
#define DIRCTRLCOLONNA3OUT          DIRCTRLCOLONNA &= CLEARCTRLCOLONNA3  // TRA2

#define DIRCTRLCOLONNA4IN           DIRCTRLCOLONNA |= CHECKCTRLCOLONNA4  // TRA3
#define DIRCTRLCOLONNA4OUT          DIRCTRLCOLONNA &= CLEARCTRLCOLONNA4  // TRA3

#define ALLCTRLCOLONNALOW           CTRLCOLONNALAT &= CLEARALLCTRLCOLONNA   // RA3-0 = 0
#define CTRLCOLONNA1HIGH            CTRLCOLONNABIT |= CHECKCTRLCOLONNA1     // RA0 = 1
#define CTRLCOLONNA2HIGH            CTRLCOLONNABIT |= CHECKCTRLCOLONNA2     // RA1 = 1
#define CTRLCOLONNA3HIGH            CTRLCOLONNABIT |= CHECKCTRLCOLONNA3     // RA2 = 1
#define CTRLCOLONNA4HIGH            CTRLCOLONNABIT |= CHECKCTRLCOLONNA4     // RA3 = 1

#ifndef _TAST_4RX4C_H
  #define _TAST_4RX4C_H
  #endif

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Fine file                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
