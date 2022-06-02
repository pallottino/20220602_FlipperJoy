
/*! @date 27 Maggio 2022 Real Time Clock Template */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         marcoangelo.r@alice.it //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Prototipi delle funzioni //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

void initRtc48 (void);
void initPrescaler (void);

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Definizioni per il precompilatore //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#if defined(_18F2550_H_) || defined(_PIC18F2550_H_) || defined(_18F2455_H_) || defined(_PIC18F2455_H_)
#define MICRO28PIN
#elif defined(_18F4550_H_) || defined(_PIC18F4550_H_) || defined(_18F4455_H_) || defined(_PIC18F4455_H_)
#define MICRO40PIN
#else
#error "Definire micro"
#endif

