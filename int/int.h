
/*! @date 27 Maggio 2022 Int Template */

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

void initTimer0 (void);
void enaTimer0 (uint16_t time);
void disTimer0 (void);
void initTimer1 (void);
void enaTimer1 (uint16_t time);
void disTimer1 (void);
void initTimer2 (void);
void enaTimer2 (uint8_t time);
void disTimer2 (void);
void initTimer3 (void);
void enaTimer3 (uint16_t time);
void disTimer3 (void);
void initCcp1 (void);
// void initExtInt (void);
void enaInt0Falling (void);
void enaInt0Rising (void);
void disInt0 (void);
void enaIntRbOnChange (void);
void disIntRbOnChange (void);
void initWdt (void);

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Definizioni per Interrupt e Timer //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

// INTCON REGISTER
// Note: Interrupt flag bits are set when an interrupt condition occurs,
// regardless of the state of its corresponding enable bit or the global
// enable bit. User software should ensure the appropriate interrupt flag
// bits are clear prior to enabling an interrupt. This feature allows for
// software polling.
//
// bit 7    GIE/GIEH:           Global Interrupt Enable bit
//                              When IPEN = 0:
//                                1 = Enables all unmasked interrupts
//                                0 = Disables all interrupts
//                              When IPEN = 1:
//                                1 = Enables all high priority interrupts
//                                0 = Disables all interrupts
// bit 6    PEIE/GIEL:          Peripheral Interrupt Enable bit
//                              When IPEN = 0:
//                                1 = Enables all unmasked peripheral
//                                interrupts 0 = Disables all peripheral
//                                interrupts
//                              When IPEN = 1:
//                                1 = Enables all low priority peripheral
//                                interrupts 0 = Disables all low priority
//                                peripheral interrupts
// bit 5    TMR0IE:             TMR0 Overflow Interrupt Enable bit
//                                1 = Enables the TMR0 overflow interrupt
//                                0 = Disables the TMR0 overflow interrupt
// bit 4    INT0IE:             INT0 External Interrupt Enable bit
//                                1 = Enables the INT0 external interrupt
//                                0 = Disables the INT0 external interrupt
// bit 3    RBIE:               RB Port Change Interrupt Enable bit
//                                1 = Enables the RB port change interrupt
//                                0 = Disables the RB port change interrupt
// bit 2    TMR0IF:             TMR0 Overflow Interrupt Flag bit
//                                1 = TMR0 register has overflowed (must be
//                                cleared soft) 0 = TMR0 register did not
//                                overflow
// bit 1    INT0IF:             INT0 External Interrupt Flag bit
//                                1 = The INT0 external interrupt occurred
//                                (must be
//                                    cleared in software)
//                                0 = The INT0 external interrupt did not occur
// bit 0    RBIF:               RB Port Change Interrupt Flag bit
//                                1 = At least one of the RB7:RB4 pins changed
//                                state
//                                    (must be cleared in software)
//                                0 = None of the RB7:RB4 pins have changed
//                                state

#define EI INTCON |= 0b10000000 // Bit 7 =  1 abilita global interrupt
#define DI INTCON &= 0b01111111 // Bit 7 =  0 disabilita global interrupt
#define PHEINTON     INTCON |= 0b01000000 // Bit 6 =  1 disabilita pheripheralinterrupt
#define PHEINTOFF    INTCON &= 0b10111111 // Bit 6 =  0 abilita pheripheral interrupt
#define TIMER0INTON  INTCON |= 0b00100000 // Bit 5 =  1 abilita Timer 0 overflow interrupt
#define TIMER0INTOFF INTCON &= 0b11011111 // Bit 5 =  0 disabilita Timer 0 overflow interrupt
#define TIMER0INTISON (INTCON & 0b00100000) != 0 // Bit 5 == 1 Timer 0 Int Ovf e' abilitato
#define TIMER0INTISOFF (INTCON & 0b00100000) == 0 // Bit 5 == 0 Timer 0 Int Ovf e' disabilitato
#define INT0ON INTCON |= 0b00010000  // Bit 4 =  1 abilita Int 0 interrupt
#define INT0OFF INTCON &= 0b11101111 // Bit 4 =  0 disabilita Int 0 interrupt
#define INT0ISON     (INTCON & 0b00010000) != 0 // Bit 4 == 1 Int 0 interrupt e' abilitato
#define INT0ISOFF    (INTCON & 0b00010000) == 0 // Bit 4 == 0 Int 0 interrupt e' disabilitato
#define INTRBONCHANGEON INTCON |= 0b00001000 // Bit 3 =  1 abilita interrupt on change di porta B
#define INTRBONCHANGEOFF INTCON &= 0b11110111 // Bit 3 =  0 disabilita interrupt on change di porta B
#define INTRBONCHANGEISON (INTCON & 0b00001000)  != 0 // Bit 3 == 1 interrupt on change di porta B e' abilitato
#define INTRBONCHANGEISOFF (INTCON & 0b00001000)  == 0 // Bit 3 == 0 interrupt on change di porta B e' disabilitato
#define TIMER0IFISSET (INTCON & 0b00000100) != 0 // Bit 2 != 0 Timer 0 Int Ovf e' settato
#define TIMER0IFISCLEAR (INTCON & 0b00000100) == 0 // Bit 2 == 0 Timer 0 Int Ovf e' clear
#define CLEARTIMER0IF INTCON &= 0b11111011 // Bit 2 =  0 Clear flag overflow interrupt TIMER0
#define INT0IFISSET  (INTCON & 0b00000010) != 0 // Bit 1 == 1 Int 0 flag is set
#define INT0IFISCLEAR (INTCON & 0b11111101) == 0           // Bit 1 == 0 Int 0 flag is clear
#define CLEARINT0IF INTCON &= 0b11111101 // Bit 1 =  0 Int 0 flag reset
#define RBONCHANGEIFISSET (INTCON & 0b00000001)  != 0 // Bit 0 == 1 porta B ha cambiato stato (interrupt on change)
#define RBONCHANGEIFISCLEAR (INTCON & 0b00000001) == 0 // Bit 0 == 0
#define CLEARRBONCHANGEIF INTCON &= 0b11111110 // Bit 0 =  0 Reset flag Interrupt on change di porta B
#define READPORTRBONCHANGE PORTB = PORTB // Reset per lettura Int Rb On Change

// PIE1:                        PERIPHERAL INTERRUPT ENABLE REGISTER 1
// bit 7 PSPIE(1):              Parallel Slave Port Read/Write Interrupt Enable
// bit
//                                1 = Enables the PSP read/write interrupt
//                                0 = Disables the PSP read/write interrupt
// bit 6 ADIE:                  A/D Converter Interrupt Enable bit
//                                1 = Enables the A/D interrupt
//                                0 = Disables the A/D interrupt
// bit 5 RCIE:                  USART Receive Interrupt Enable bit
//                                1 = Enables the USART receive interrupt
//                                0 = Disables the USART receive interrupt
// bit 4 TXIE:                  USART Transmit Interrupt Enable bit
//                                1 = Enables the USART transmit interrupt
//                                0 = Disables the USART transmit interrupt
// bit 3 SSPIE:                 Master Synchronous Serial Port Interrupt Enable
// bit
//                                1 = Enables the MSSP interrupt
//                                0 = Disables the MSSP interrupt
// bit 2 CCP1IE:                CCP1 Interrupt Enable bit
//                                1 = Enables the CCP1 interrupt
//                                0 = Disables the CCP1 interrupt
// bit 1 TMR2IE:                TMR2 to PR2 Match Interrupt Enable bit
//                                1 = Enables the TMR2 to PR2 match interrupt
//                                0 = Disables the TMR2 to PR2 match interrupt
// bit 0 TMR1IE:                TMR1 Overflow Interrupt Enable bit
//                                1 = Enables the TMR1 overflow interrupt
//                                0 = Disables the TMR1 overflow interrupt
//

#define INTADON      PIE1 |= 0b01000000 // Bit 6 (ADIE)    = 1    Abilita int A/D converter
#define INTADOFF     PIE1 &= 0b10111111 // Bit 6 (ADIE)    = 0 Disabilita int A/D converter
#define RXUSARTINTON PIE1 |= 0b00100000 // Bit 5 (RCIE)    = 1    Abilita int Periferica Usart in
                     // Rx
#define RXUSARTINTOFF PIE1 &= 0b11011111 // Bit 5 (RCIE)    = 0 Disabilita int Periferica Usart in
                     // Rx
#define TXUSARTINTON PIE1 |= 0b00010000                    // Tx
#define TXUSARTINTOFF PIE1 &= 0b11101111 // Bit 4 (TXIE)    = 0 Disabilita int Periferica Usart in Tx
#define SSPINTON     PIE1 |= 0b00001000                    
#define SSPINTOFF    PIE1 &= 0b11110111 // Bit 3 (SSPIE)   = 0 Disabilita interrupt Parallel Slave Port 
#define SSPINTISSET  (PIE1 & 0b00001000)  != 0 // Bit 3 (SSPIE)  != 0 Int SSP (I2C) e'    Abilitato
#define SSPINTISCLEAR (PIE1 & 0b0000100)  == 0 // Bit 3 (SSPIE)  == 0 Int SSP (I2C) e' Disabilitato
#define CCP1INTON PIE1 |= 0b00000100 // Bit 2 (CCP1E)   = 1    Abilita Int Ccp1
#define CCP1INTOFF   PIE1 &= 0b11111011 // Bit 2 (CCP1E)   = 1 Disabilita Int Ccp1
#define CCP1INTISON  PIE1 & 0b0000010 
#define CCP1INTISOFF PIE1 & 0b0000010 
#define TIMER2INTON  PIE1 |= 0b00000010 
#define TIMER2INTOFF PIE1 &= 0b11111101 // Bit 1 (TMR2IE)  = 0 Disabilita Int Timer 2
#define TIMER2INTISON (PIE1 & 0b0000010) == 0 // Bit 1 (TMR2IE) == 0 Int Timer 2 e' Disabilitato
#define TIMER1INTON  PIE1 |= 0b00000001 // Bit 0 (TMR1IE) =  1    Abilita Int Timer 1
#define TIMER1INTOFF PIE1 &= 0b11111110 
#define TIMER1INTISON (PIE1 & 0b00000001) != 0 // Bit 0 (TMR1IE) =  1 Int Timer 1 e'    Abilitato
#define TIMER1INTISOFF (PIE1 & 0b00000001) == 0 // Bit 0 (TMR1IE) =  1 Int Timer 1 e' Disabilitato

//
// PIE2:              bit 7 OSCFIE:                Oscillator Fail Interrupt Enable bit
//                                1 = Enabled
//                    bit 6 CMIE:                  Comparator Interrupt Enable bit
//                                1 = Enabled
//                                0 = Disabled
// bit 5 Unimplemented:         Read as �0�
// bit 4 EEIE:                  Data EEPROM/Flash Write Operation Interrupt
// Enable bit
//                                1 = Enabled
//                                0 = Disabled
// bit 3 BCLIE:                 Bus Collision Interrupt Enable bit
//                                1 = Enabled
//                                0 = Disabled
// bit 2 HLVDIE:                High/Low-Voltage Detect Interrupt Enable bit
//                                1 = Enabled
//                                0 = Disabled
// bit 1 TMR3IE:                TMR3 Overflow Interrupt Enable bit
//                                1 = Enabled
//                                0 = Disabled
// bit 0 CCP2IE:                CCP2 Interrupt Enable bit
//                                1 = Enabled
//                                0 = Disabled
//
#define CCPINTON PIE2 |= 0b01000000 // PIE2 Bit 6 = 1 Ccp interrupt abilitato
#define CCPINTOFF    PIE2 &= 0b10111110 // PIE2 Bit 6 = 0 Ccp interrupt disabilitato
#define SSPCOLLISIONINTON PIE2 |= 0b00001000  // PIE2 Bit 3 = 1
#define SSPCOLLISIONINTOFF PIE2 &= 0b11110111 // PIE2 Bit 3 = 0
#define SSPCOLLISIONINTISSET (PIE2 & 0b11110111) != 0 // PIE2 Bit 3 != 0 SSPCOLLISION is set
#define SSPCOLLISIONINTISCLEAR (PIE2 & 0b11110111) == 0 // PIE2 Bit 3 == 0 SSPCOLLISION is clear
#define TIMER3INTON  PIE2 |= 0b00000010 // PIE2 Bit 1 = 1 Abilita interrupt da Timer 3
#define TIMER3INTOFF PIE2 &= 0b11111101 // PIE2 Bit 1 = 0 Disabilita interrupt da Timer 3
#define TIMER3INTISO (PIE2 & 0b00000010)  != 0 // PIE2 Bit 1 != 0 Interrupt da Timer 3 e' abilitato
#define TIMER3INTISOFF (PIE2 & 0b00000010)  == 0 // PIE2 Bit 1 == 0 Interrupt da Timer 3 e' disabilitato
#define CCP2INTON PIE2 |= 0b00000001  // Bit 0 (PIE2) = 1 Disabilita Int Ccp2
#define CCP2INTOFF PIE2 &= 0b11111110 // Bit 0 (PIE2) = 0 Disabilita Int Ccp2
// PIE2 Bit 0 = 0 Ccp2 interrupt disabilitato
// PIR1:                        PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 1
// bit 7 PSPIF:                 Parallel Slave Port Read/Write Interrupt Flag
// bit(1)                                 1 = A read or a write operation has taken
//                                place (must be cleared in software) 0 = No
//                                read or write has occurred
// bit 6 ADIF:                  A/D Converter Interrupt Flag bit
//                                1 = An A/D conversion completed (must be
//                                cleared in software) 0 = The A/D conversion
//                                is not complete
// bit 5 RCIF:                  EUSART Receive Interrupt Flag bit
//                                1 = The EUSART receive buffer, RCREG, is full
//                                (cleared when RCREG is read) 0 = The EUSART
//                                receive buffer is empty
// bit 4 TXIF:                  EUSART Transmit Interrupt Flag bit
//                              1 = The EUSART transmit buffer, TXREG, is empty
//                              (cleared when TXREG is written) 0 = The EUSART
//                              transmit buffer is full
// bit 3 SSPIF:                 Master Synchronous Serial Port Interrupt Flag
// bit
//                                1 = The transmission/reception is complete
//                                (must be cleared in software) 0 = Waiting to
//                                transmit/receive
// bit 2 CCP1IF:                CCP1 Interrupt Flag bit
//                                Capture mode:
//                                  1 = A TMR1 register capture occurred (must
//                                  be cleared in software) 0 = No TMR1
//                                  register capture occurred
//                                Compare mode:
//                                  1 = A TMR1 register compare match occurred
//                                  (must be cleared in software) 0 = No TMR1
//                                  register compare match occurred
//                                PWM mode:
//                                  Unused in this mode.
// bit 1 TMR2IF:                TMR2 Overflow Interrupt Flag bit or TMR2 to PR2
// Match Interrupt Flag bit:
//                                1 = TMR2 to PR2 match occurred (must be
//                                cleared in software) 0 = No TMR2 to PR2 match
//                                occurred
// bit 0 TMR1IF:                TMR1 Overflow Interrupt Flag bit
//                                1 = TMR1 register overflowed (must be cleared
//                                in software) 0 = TMR1 register did not
//                                overflow
//
#define ADIFISSET    (PIR1 & 0b01000000) != 0 // Bit 6 (ADIF) Interrupt flag is set
#define ADIFISCLEAR  (PIR1 & 0b01000000) == 0         // Bit 6 (ADIF) Interrupt flag is clear
#define CLEARADIF PIR1 &= 0b10111111 // Bit 6 (ADIF) Interrupt flag clear
#define CLEARRXUSARTIF PIR1 &= 0b11011111 // Bit 5 (RCIF) = 0 Clear Flag interrupt Rx Usart
#define RXUSARTIFISSET (PIR1 & 0b00100000)  != 0 // Bit 5 (RCIF) != 0 Flag interrupt Rx Usart is Set
#define RXUSARTIFISCLEAR (PIR1 & 0b00100000)  == 0 // Bit 5 (RCIF) == 0 Flag interrupt Rx Usart is Clear
#define TXUSARTIFISEMPTY (PIR1 & 0b00010000)  != 0 // Bit 4 (TXIF)
#define TXUSARTIFISFULL (PIR1 & 0b00010000)  == 0 // Bit 4 (TXIF)
#define CLEARSSPIF   PIR1 &= 0b11110111 // Bit 3 (SSPIF) = 0 Clear Flag interrupt da SSP (I2C)
#define SSPIFISSET   (PIR1 & 0b00001000)  != 0 // Bit 3 (SSPIF) != 0 Flag interrupt da SSP (I2C) is Set
#define SSPIFISCLEAR (PIR1 & 0b00001000)  == 0 // Bit 3 (SSPIF) == 0 Flag interrupt da SSP (I2C) is Clear
#define CLEARCCP1IF PIR1 &= 0b11111011 // Bit 2 (CCP1IF) = 0
#define CCP1IFISSET  (PIR1 & 0b00000100)  != 0 // Bit 2 (CCP1IF) != 0 Flag interrupt da SSP (I2C) is Set
#define CCP1IFISCLEAR (PIR1 & 0b00000100)  == 0 // Bit 2 (CCP1IF) == 0 Flag interrupt da SSP (I2C) is Clear
#define CLEARTIMER2IF PIR1 &= 0b11111101 // Bit 1 (TMR2IF) =  0 Reset TIMER2 flag interrupt
#define TIMER2IFISSET (PIR1 & 0b00000010)  != 0 // Bit 1 (TMR2IF) == 1 TIMER2 flag interrupt is SET
#define TIMER2IFISCLEAR (PIR1 & 0b00000010)  == 0 // Bit 1 (TMR2IF) == 0 TIMER2 flag interrupt is SET
#define CLEARTIMER1IF PIR1 &= 0b11111110 
#define TIMER1IFISSET (PIR1 & 0b00000001)  != 0 // Bit 0 (TMR1IF) == 1 TIMER1 flag interrupt is SET
#define TIMER1IFISCLEAR (PIR1 & 0b0000000)  == 0 // Bit 0 (TMR1IF) == 0 TIMER1 flag interrupt is CLEAR

// T2CON: TIMER2 CONTROL REGISTER (ADDRESS 12h)
// bit 7:   Unimplemented: Read as '0'
// bit 6-3: TOUTPS3:TOUTPS0: Timer2 Output Postscale Select bits
//          0000 = 1:1 Postscale
//          0001 = 1:2 Postscale
//          0010 = 1:3 Postscale
//          1111 = 1:16 Postscale
// bit 2:   TMR2ON: Timer2 On bit
//          1 = Timer2 is on
//          0 = Timer2 is off
// bit 1-0: T2CKPS1:T2CKPS0: Timer2 Clock Prescaler Select bits
//          00 = Prescaler is 1
//          01 = Prescaler is 4
//          1x = Prescaler is 16
//
#define TIMER2ON T2CON |= 0b00000100  // Bit 2 (TMR2ON) == 1 abilita Timer 2
#define TIMER2OFF T2CON &= 0b11111011 // Bit 2 (TMR2ON) == 0 disabilita Timer 2
#define TIMER2ISON   (T2CON & 0b00000100) != 0 // Bit 2 == 1 Timer 2 e' abilitato
#define TIMER2ISOFF  (T2CON & 0b00000100) == 0 // Bit 2 == 0 Timer 2 e' disabilitato
#define TIMER2POST016 T2CON |= 0b01111000 // Bit 6-3 = 01111000 postscaler value = 1:16
#define TIMER2POST015 T2CON |= 0b01110000 // Bit 6-3 = 01111000 postscaler value = 1:15
#define TIMER2POST014 T2CON |= 0b01101000 // Bit 6-3 = 01101000 postscaler value = 1:14
#define TIMER2POST013 T2CON |= 0b01100000 // Bit 6-3 = 01100000 postscaler value = 1:13
#define TIMER2POST012 T2CON |= 0b01011000 // Bit 6-3 = 01011000 postscaler value = 1:12
#define TIMER2POST011 T2CON |= 0b01010000 // Bit 6-3 = 01010000 postscaler value = 1:11
#define TIMER2POST010 T2CON |= 0b01001000 // Bit 6-3 = 01001000 postscaler value = 1:10
#define TIMER2POST009 T2CON |= 0b01000000 // Bit 6-3 = 01000000 postscaler value = 1: 9
#define TIMER2POST008 T2CON |= 0b00111000 // Bit 6-3 = 00111000 postscaler value = 1: 8
#define TIMER2POST007 T2CON |= 0b00110000 // Bit 6-3 = 00110000 postscaler value = 1: 7
#define TIMER2POST006 T2CON |= 0b00101000 // Bit 6-3 = 00101000 postscaler value = 1: 6
#define TIMER2POST005 T2CON |= 0b00100000 // Bit 6-3 = 00100000 postscaler value = 1: 5
#define TIMER2POST004 T2CON |= 0b00011000 // Bit 6-3 = 00011000 postscaler value = 1: 4
#define TIMER2POST003 T2CON |= 0b00010000 // Bit 6-3 = 00010000 postscaler value = 1: 3
#define TIMER2POST002 T2CON |= 0b00001000 // Bit 6-3 = 00001000 postscaler value = 1: 2
#define TIMER2POSTRESET T2CON &= 0b10000111 // Bit 6/3 = 0000 postscaler value 1: 1 (Reset
                      // postscaler)

#define TIMER2PRE016 T2CON |= 0b00000010 // Bit 1-0 = 11   prescaler value  1:16
#define TIMER2PRE004 T2CON |= 0b00000001 // bit 1-0 = 01   prescaler value  1: 4
#define TIMER2PRERESET T2CON &= 0b11111100 // Bit 1-0 = 00   prescaler value  1: 1 (Reset prescaler)

#if defined(_PIC_H_)
#define WDTON WDTE_ON
#define WDTOFF WDTE_OFF

// Interrupt TIMER0 Ovf
// OPTION_REG               OPTION REGISTER (ADDRESS: 81h, 181h)  
// bit 7 RBPU neg:          PORTB Pull-up Enable bit
//                            1 = PORTB pull-ups are disabled
//                            0 = PORTB pull-ups are enabled by individual port
//                            latch values
// bit 6 INTEDG:            Interrupt Edge Select bit
//                            1 = Interrupt on rising edge of RB0/INT pin
//                            0 = Interrupt on falling edge of RB0/INT pin
//
// T0CON:                   TIMER0 CONTROL REGISTER (Reset = 0xff)
//
// bit 7 TMR0ON:            Timer0 On/Off Control bit
//                            1 = Enables Timer0
//                            0 = Stops Timer0
// bit 6 INTEDG:            INTEDG: Interrupt Edge Select bit
//                            1 = Interrupt on rising edge of RB0/INT pin
//                            0 = Interrupt on falling edge of RB0/INT pin
// bit 6 T08BIT:            Timer0 8-bit/16-bit Control bit
//                            1 = Timer0 is configured as an 8-bit
//                            timer/counter 0 = Timer0 is configured as a
//                            16-bit timer/counter
// bit 5 T0CS:              TMR0 Clock Source Select bit
//                            1 = Transition on RA4/T0CKI/CMP2 pin
//                            0 = Internal instruction cycle clock (CLKOUT)
// bit 4 T0SE:              TMR0 Source Edge Select bit
//                            1 = Increment on high-to-low transition on
//                            RA4/T0CKI/CMP2 pin 0 = Increment on low-to-high
//                            transition on RA4/T0CKI/CMP2 pin
// bit 3 PSA:               Prescaler Assignment bit
//                            1 = Prescaler is assigned to the WDT
//                            0 = Prescaler is assigned to the Timer0 module
//                            (Valori prescaler / 2)
// bit 2-0 PS<2:0>: Prescaler Rate Select bits
//                          Bit Value    TMR0 Rate     WDT Rate
//                            111         1 : 256       1 : 128
//                            110         1 : 128       1 : 64
//                            101         1 : 64        1 : 32
//                            100         1 : 32        1 : 16
//                            011         1 : 16        1 : 8
//                            010         1 : 8         1 : 4
//                            001         1 : 4         1 : 2
//                            000         1 : 2         1 : 1
//
#define PULLUPON     OPTION_REG &= 0b01111111 // Bit 7 = 0 Pull-up su porta B Abilitate
#define PULLUPOFF    OPTION_REG |= 0b10000000 // Bit 7 = 1 Pull-up su porta B Disabilitate
#define INT0EDGERIS  OPTION_REG |= 0b01000000 // Bit 6 = 1 per Rising  edge Int 0 (0->1)
#define INT0EDGEFAL  OPTION_REG &= 0b10111111 // Bit 6 = 0 per Falling  edge Int 0 (0->1)
#define TIMER0CLOCKOSC OPTION_REG |= 0b00100000 // Bit 5 (T0CS)  = 1 Timer 0 Clock Source External
                           // (RA 4 T0CKI)
#define TIMER0CLOCKXTAL OPTION_REG &= 0b11011111 // Bit 5 (T0CS)  = 0 Timer 0 Clock Source internal
                           // (CLK0 = XTAL / 4)
#define TIMER0EDGERIS OPTION_REG &= 0b11101111 // Bit 4 = 0 Timer0 increment on Rising edge
#define TIMER0EDGEFAL OPTION_REG |= 0b00010000 // Bit 4 = 1 Timer0 increment on Falling edge
#define PRE2TIMER0 OPTION_REG &= 0b11110111 // Bit 3 = 0 per prescaler a Timer0
#define PRE2WDT OPTION_REG |= 0b00001000    // Bit 3 = 1 per prescaler a Wdt
#define TIMER0PRE256 OPTION_REG |= 0b00000111 // Bit 2-0 = 111 = 1:256 prescale value (WDT = 1/2)
#define TIMER0PRE128 OPTION_REG |= 0b00000110 // Bit 2-0 = 110 = 1:128 prescale value
#define TIMER0PRE064 OPTION_REG |= 0b00000101 // Bit 2-0 = 101 = 1:64 prescale value
#define TIMER0PRE032 OPTION_REG |= 0b00000100 // Bit 2-0 = 100 = 1:32 prescale value
#define TIMER0PRE016 OPTION_REG |= 0b00000011 // Bit 2-0 = 011 = 1:16 prescale value
#define TIMER0PRE008 OPTION_REG |= 0b00000010 // Bit 2-0 = 010 = 1:8 prescale value
#define TIMER0PRE004 OPTION_REG |= 0b00000001 // Bit 2-0 = 001 = 1:4 prescale value
#define TIMER0PRERESET OPTION_REG &= 0b11111000 // Bit 2-0 = 000 = 1:2 prescale value Clear bit 2-0
                           // Timer0 Prscaler RESET
#define CLEARWDT CLRWDT ()
#else
#define TIMER0ON T0CON |= 0b10000000    // Bit 7 = 1 abilita Timer 0
#define TIMER0OFF T0CON &= 0b01111111   // Bit 7 = 0 disabilita Timer 0
#define TIMER016BIT T0CON &= 0b10111111 // Bit 6 = 0 abilita contatore a 16 bit
#define TIMER08BIT   T0CON |= 0b01000000 // Bit 6 = 1 abilita con contatore a  8 bit
#define TIMER0CLOCKOSC T0CON |= 0b00100000 // Bit 5 = 1 Timer 0 Clock Source External (RA 4 T0CKI)
#define TIMER0CLOCKXTAL T0CON &= 0b11011111 // Bit 5 = 0 Timer 0 Clock Source internal (CLK0 = XTAL /
                      // 4)
#define TIMER0EDGERIS T0CON &= 0b11101111 // Bit 4 = 0 Timer0 increment on Rising edge
#define TIMER0EDGEFAL T0CON |= 0b00010000 // Bit 4 = 1 Timer0 increment on Falling edge
#define PRETIMER0OFF T0CON |= 0b00001000 // Bit 3 = 1 prescaler bypass  (Timer 0 Prescaler 1:1)
#define PRETIMER0ON T0CON &= 0b11110111 // Bit 3 = 0 prescaler Timer0 ON
#define TIMER0PRE256 T0CON |= 0b00000111 // Bit 2-0 = 111 = 1:256 prescale value (WDT = 1/2)
#define TIMER0PRE128 T0CON |= 0b00000110 // Bit 2-0 = 110 = 1:128 prescale value
#define TIMER0PRE064 T0CON |= 0b00000101 // Bit 2-0 = 101 = 1:64 prescale value
#define TIMER0PRE032 T0CON |= 0b00000100 // Bit 2-0 = 100 = 1:32 prescale value
#define TIMER0PRE016 T0CON |= 0b00000011 // Bit 2-0 = 011 = 1:16 prescale value
#define TIMER0PRE008 T0CON |= 0b00000010 // Bit 2-0 = 010 = 1:8 prescale value
#define TIMER0PRE004 T0CON |= 0b00000001 // Bit 2-0 = 001 = 1:4 prescale value
#define TIMER0PRERESET T0CON &= 0b11111000 // Bit 2-0 = 000 = 1:2 prescale value RESET (Clear bit
                      // 2-0 Timer0 Prescaler)
#define CLEARWDT ClrWdt ()

//
// T3CON:   TIMER3 CONTROL REGISTER
// bit 7    RD16:   16-bit Read/Write Mode Enable bit
//                  1 = Enables register Read/Write of Timer3 in one 16-bit op
//                  0 = Enables register Read/Write of Timer3 in two 8-bit op
// bit 6,3  T3CCP2:T3CCP1: Timer3 and Timer1 to CCPx Enable bits
//                  1x = Timer3 is the capture/compare clock source for the CCP
//                  modules 01 = Timer3 is the capture/compare clock source for
//                  CCP2;
//                       Timer1 is the capture/compare clock source for CCP1
//                  00 = Timer1 is the capture/compare clock source for the CCP
//                  modules
// bit 5-4  T3CKPS1:T1CKPS0: Timer3 Input Clock Prescale Select bits
//                  11 = 1:8 Prescale value
//                  10 = 1:4 Prescale value
//                  01 = 1:2 Prescale value
//                  00 = 1:1 Prescale value
// bit 2    T3SYNC: Timer1 External Clock Input Synchronization control bit
//                      (Not usable if the device clock comes from
//                      Timer1/Timer3.)
//                  When TMR3CS = 1:
//                  1 = Do not synchronize external clock input
//                  0 = Synchronize external clock input
//                  When TMR3CS = 0:
//                  This bit is ignored. Timer3 uses the internal clock when
//                      TMR3CS = 0.
// bit 1    TMR3CS: Timer3 Clock Source Select bit
//                  1 = External clock input from Timer3 oscillator or T13CKI
//                  (on the rising edge after the first
//                      falling edge)
//                  0 = Internal clock (FOSC/4)
// bit 0    TMR3ON: Timer3 On bit
//                  1 = Enables Timer3
//                  0 = Stops Timer3
//
#define TIMER3_16BIT T3CON |= 0b10000000 // Bit 7 = 1 Enables register Read/Write of Timer1 in one
                      // 16-bit op
#define TIMER3_08BIT T3CON &= 0b01111111 // Bit 7 = 0 Enables register Read/Write of Timer1 in two
                      // 8-bit op
#define TIMER3CCPSOURCE T3CON &= 0b10110111 // Bit 6 = 0 e Bit 3 == 0 (Reset register)
#define TIMER3CCP1TIMER3CCP2SOURCE T3CON |= 0b00001000 // Bit 6 = 0 bit 3 == 1
#define TIMER3PRERESET T3CON &= 0b11001111 // Bit 5-4 = 00 prescaler value 1:1 (Reset prescaler)
#define TIMER3PRE002 T3CON |= 0b00010000  // Bit 5-4 = 01 prescaler value 1:2
#define TIMER3PRE004 T3CON |= 0b00100000  // Bit 5-4 = 10 prescaler value 1:4
#define TIMER3PRE008 T3CON |= 0b00110000  // Bit 5-4 = 11 prescaler value 1:8
#define TIMER3SYNCON T3CON &= 0b11111011  // Bit 2 = 0 External clock not sync
#define TIMER3SYNCOFF T3CON |= 0b00000100 // Bit 2 = 1 External clock     sync
#define TIMER3CLOCKSOURCEEXT T3CON |= 0b00000010 // Bit   1 (TMR1CS)  = 1 Timer1Clock = External clock
                      // from pin RC0/T1OSO/T13CKI (on the rising edge)
#define TIMER3CLOCKSOURCEINT T3CON &= 0b11111101 // Bit   1 (TMR1CS)  = 0 Timer1Clock = Internal clock
                      // (FOSC/4)
#define TIMER3ON T3CON |= 0b00000001            // Bit 0 = 1 abilita Timer 3
#define TIMER3OFF T3CON &= 0b11111110           // Bit 0 = 0 disabilita Timer 3
#define TIMER3ISON (T3CON & 0b00000001) != 0  // Timer 3 e' abilitato
#define TIMER3ISOFF (T3CON & 0b00000001) == 0 // Timer 3 e' disabilitato
#define TIMER3CCPBOTH T3CON &= 0b01000000 // Bit6 = 1 e bit3 = don't care = Timer3 is the
                      // capture/compare clock source for the CCP modules
// CCPxCON:                   ECCP CONTROL REGISTER (40/44-PIN CTRLDEVICES)
// bit 7-6                    P1M1:P1M0: Enhanced PWM Output Configuration bits
//                              If CCPxM3:CCPxM2 = 00, 01, 10:
//                              xx = P1A assigned as Capture/Compare
//                              input/output; P1B, P1C, P1D assigned as port
//                              pins If CCPxM3:CCPxM2 = 11: 00 = Single output:
//                              P1A modulated; P1B, P1C, P1D assigned as port
//                              pins 01 = Full-bridge output forward: P1D
//                              modulated; P1A active; P1B, P1C inactive 10 =
//                              Half-bridge output: P1A, P1B modulated with
//                              dead-band control; P1C, P1D assigned as port
//                              pins 11 = Full-bridge output reverse: P1B
//                              modulated; P1C active; P1A, P1D inactive
// bit 5-4                    DCxB1:DCxB0: PWM Duty Cycle Bit 1 and Bit 0
//                              Capture mode:
//                                Unused.
//                              Compare mode:
//                                Unused.
//                              PWM mode:
//                                These bits are the two LSbs of the 10-bit
//                                PWM duty cycle. The eight MSbs of the
//                                duty cycle are found in CCPR1L.
// bit 3-0                    CCP1M3:CCP1M0: Enhanced CCP Mode Select bits
//                              0000 = Capture/Compare/PWM off (resets ECCP
//                              module) 0001 = Reserved 0010 = Compare mode,
//                              toggle output on match 0011 = Capture mode 0100
//                              = Capture mode, every falling edge 0101 =
//                              Capture mode, every rising edge 0110 = Capture
//                              mode, every 4th rising edge 0111 = Capture
//                              mode, every 16th rising edge 1000 = Compare
//                              mode, initialize CCP1 pin low, set output on
//                              compare match (set CCP1IF) 1001 = Compare mode,
//                              initialize CCP1 pin high, clear output on
//                              compare match (set CCP1IF) 1010 = Compare mode,
//                              generate software interrupt only, CCP1 pin
//                              reverts to I/O state 1011 = Compare mode,
//                              trigger special event (CCP1 resets TMR1 or
//                              TMR3, sets CCP1IF bit) 1100 = PWM mode: P1A,
//                              P1C active-high; P1B, P1D active-high 1101 =
//                              PWM mode: P1A, P1C active-high; P1B, P1D
//                              active-low 1110 = PWM mode: P1A, P1C
//                              active-low; P1B, P1D active-high 1111 = PWM
//                              mode: P1A, P1C active-low; P1B, P1D active-low
//                              // Capture/Compare Module
//
#define RESETCCP1    CCP1CON &= 0b11110000 // CCP1 RESET (ECCP MODULE OFF) == CCP1PWMSTART_LL
#define RESETCCP2    CCP2CON &= 0b11110000 // CCP2 RESET (ECCP MODULE OFF) == CCP2PWMSTART_LL

// CCP1 Mode CAPTURE
#define CCP1CAPFALLINGEDGE CCP1CON &= 0b11110100
#define CCP1CAPISFALLINGEDGE (CCP1CON & 0b00000100) != 0
#define CCP1CAP1RISINGEDGE CCP1CON |= 0b00000101
#define CCP1CAP4RISINGEDGE CCP1CON |= 0b00000110
#define CCP1CAP16RISINGEDGE CCP1CON |= 0b00000111
#define CCP1CAPIS1RISINGEDGE (CCP1CON & 0b00000101) != 0
#define CCP1CAPIS4RISINGEDGE (CCP1CON & 0b00000110) != 0
#define CCP1CAPIS16RISINGEDGE (CCP1CON & 0b00000111) != 0

// Mode COMPARE
#define CCP1CMPSTARTLOW CCP1CON                |= 0b00001000 // Start Output LOW -> HIGH on compare Match (set CCP1IF)
#define CCP1CMPSTARTHIGH CCP1CON                |= 0b00001001 // Start Output HIGH -> LOW on compare Match (set CCP1IF)
#define CCP1CMPINTONLY CCP1CON |= 0b00001010 // Genera CCP1 Interrupt + CCP1 = I/O Pin
#define CCP1CMPSPECIAL CCP1CON |= 0b00001011 // Trigger special event (CCP1 resets TMR1 or TMR3,
                        // sets CCP1IF bit)

// Mode PWM
#define CCP1PWMSTART_HH CCP1CON |= 0b00001100 // Bit 3-0 (CCP1M3:CCPxM0) = 1100 PWM mode: P1A, P1C
                        // active-High; P1B, P1D Active-High
#define CCP1PWMSTART_HL CCP1CON |= 0b00001101 // Bit 3-0 (CCP1M3:CCPxM0) = 1101 PWM mode: P1A, P1C
                        // active-High; P1B, P1D Active-Low
#define CCP1PWMSTART_LH CCP1CON |= 0b00001110 // Bit 3-0 (CCP1M3:CCPxM0) = 1110 PWM mode: P1A, P1C
                        // active-low; P1B, P1D Active-High
#define CCP1PWMSTART_LL RESETCCP1 // Bit 3-0 (CCP1M3:CCPxM0) = 1111 PWM mode: P1A, P1C active-low;
            // P1B, P1D Active-Low
#define CLEARCCP1DUTYLSB CCP1CON |= 0b00110000 // Bit 5-4 (DCxB1:DCxB0)   = 00   PWM Duty Cycle Bit 1
                        // and Bit 0 for CCPx Module

// CCP2 Mode CAPTURE
#define CCP2CAPFALLINGEDGE CCP2CON &= 0b11110100
#define CCP2CAPISFALLINGEDGE (CCP2CON & 0b00000100) != 0
#define CCP2CAP1RISINGEDGE CCP2CON |= 0b00000101
#define CCP2CAP4RISINGEDGE CCP2CON |= 0b00000110
#define CCP2CAP16RISINGEDGE CCP2CON |= 0b00000111
#define CCP2CAPIS1RISINGEDGE (CCP2CON & 0b00000101) != 0
#define CCP2CAPIS4RISINGEDGE (CCP2CON & 0b00000110) != 0
#define CCP2CAPIS16RISINGEDGE (CCP2CON & 0b00000111) != 0

// Mode COMPARE
#define CCP2CMPSTARTLOW CCP2CON                |= 0b00001000 // Start Output LOW -> HIGH on compare Match (set CCP2IF)
#define CCP2CMPSTARTHIGH CCP2CON                |= 0b00001001 // Start Output HIGH -> LOW on compare Match (set CCP2IF)
#define CCP2CMPINTONLY CCP2CON |= 0b00001010 // Genera CCP2 Interrupt + CCP2 = I/O Pin
#define CCP2CMPSPECIAL CCP2CON |= 0b00001011 // Trigger special event (CCP2 resets TMR1 or TMR3,
                        // sets CCP2IF bit)

// Mode PWM
#define CCP2PWMSTART_HH CCP2CON |= 0b00001100 // Bit 3-0 (CCP2M3:CCPxM0) = 1100 PWM mode: P1A, P1C
                        // active-High; P1B, P1D Active-High
#define CCP2PWMSTART_HL CCP2CON |= 0b00001101 // Bit 3-0 (CCP2M3:CCPxM0) = 1101 PWM mode: P1A, P1C
                        // active-High; P1B, P1D Active-Low
#define CCP2PWMSTART_LH CCP2CON |= 0b00001110 // Bit 3-0 (CCP2M3:CCPxM0) = 1110 PWM mode: P1A, P1C
                        // active-low; P1B, P1D Active-High
#define CCP2PWMSTART_LL RESETCCP2 // Bit 3-0 (CCP2M3:CCPxM0) = 1111 PWM mode: P1A, P1C active-low;
            // P1B, P1D Active-Low
#define CLEARCCP2DUTYLSB CCP2CON |= 0b00110000 // Bit 5-4 (DCxB1:DCxB0)   = 00   PWM Duty Cycle Bit 1
                        // and Bit 0 for CCPx Module

// INTCON2:                   INTERRUPT CONTROL REGISTER 2
// bit 7 RBPU:                PORTB Pull-up Enable bit
//                              1 = All PORTB pull-ups are disabled
//                              0 = PORTB pull-ups are enabled by individual
//                              port latch values
// bit 6 INTEDG0:             External Interrupt 0 Edge Select bit
//                              1 = Interrupt on rising edge
//                              0 = Interrupt on falling edge
// bit 5 INTEDG1:             External Interrupt 1 Edge Select bit
//                              1 = Interrupt on rising edge
//                              0 = Interrupt on falling edge
// bit 4 INTEDG2:             External Interrupt 2 Edge Select bit
//                              1 = Interrupt on rising edge
//                              0 = Interrupt on falling edge
// bit 3 Unimplemented: Read as �0�
// bit 2 TMR0IP:              TMR0 Overflow Interrupt Priority bit
//                              1 = High priority
//                              0 = Low priority
// bit 1 Unimplemented: Read as �0�
// bit 0 RBIP:                RB Port Change Interrupt Priority bit
//                              1 = High priority
//                              0 = Low priority
// INT2 (External Int 2)
#define PULLUPON     INTCON2 &= 0b01111111 // Bit 7 = 0 Pull-up su porta B Abilitate
#define PULLUPOFF    INTCON2 |= 0b10000000 // Bit 7 = 1 Pull-up su porta B Disabilitate
#define INT0EDGERIS  INTCON2 |= 0b01000000 // Bit 6 = 1 Rising   edge Int 0 (0 -> 1)
#define INT0EDGEFAL  INTCON2 &= 0b10111111 // Bit 6 = 0 Falling  edge Int 0 (1 -> 0)
#define INT1EDGERIS  INTCON2 |= 0b00100000 // Bit 5 = 1 Rising   edge Int 1 (0 -> 1)
#define INT1EDGEFAL  INTCON2 &= 0b11011111 // Bit 5 = 0 Falling  edge Int 1 (1 -> 0)
#define INT2EDGERIS  INTCON2 |= 0b00010000 // Bit 4 = 1 Rising   edge Int 2 (0 -> 1)
#define INT2EDGEFAL  INTCON2 &= 0b11101111 // Bit 4 = 0 Falling  edge Int 2 (1 -> 0)

// INTCON3:                   INTERRUPT CONTROL REGISTER 3
// bit 7 INT2IP:              INT2 External Interrupt Priority bit
//                              1 = High priority
//                              0 = Low priority
// bit 6 INT1IP:              INT1 External Interrupt Priority bit
//                              1 = High priority
//                              0 = Low priority
// bit 5 Unimplemented:       Read as �0�
// bit 4 INT2IE:              INT2 External Interrupt Enable bit
//                              1 = Enables the INT2 external interrupt
//                              0 = Disables the INT2 external interrupt
// bit 3 INT1IE:              INT1 External Interrupt Enable bit
//                              1 = Enables the INT1 external interrupt
//                              0 = Disables the INT1 external interrupt
// bit 2 Unimplemented:       Read as �0�
// bit 1 INT2IF:              INT2 External Interrupt Flag bit
//                              1 = The INT2 external interrupt occurred (must
//                              be cleared in software) 0 = The INT2 external
//                              interrupt did not occur
// bit 0 INT1IF:              INT1 External Interrupt Flag bit
//                              1 = The INT1 external interrupt occurred (must
//                              be cleared in software) 0 = The INT1 external
//                              interrupt did not occur
//

#define INT2ON       INTCON3 |= 0b00010000 // Bit 4 =  1 enable Int 2 external interrupt
#define INT2OFF      INTCON3 &= 0b11101111 // Bit 4 =  0 enable Int 2 external interrupt
#define INT2ISON     (INTCON3 & 0b00010000)  != 0 // Bit 4 != 0 Int 2 external interrupt is enabled
#define INT2ISOFF    (INTCON3 & 0b00010000)  == 0 // Bit 4 == 0 Int 2 external interrupt is disabled
#define CLEARINT2IF  INTCON3 &= 0b11111101 // Bit 1 =  0 Int 2 external interrupt Flag
#define INT2IFISSET  (INTCON3 & 0b00000010) != 0 // Bit 1 != 0 Int 2 external interrupt is Set
#define INT2IFISCLEAR (INTCON3 & 0b00000010) == 0 // Bit 1 == 0 Int 2 external interrupt is Clear

#define INT1ON       INTCON3 |= 0b00001000 // Bit 3 =  1 enable Int 1 external interrupt
#define INT1OFF      INTCON3 &= 0b11110111 // Bit 3 =  0 enable Int 1 external interrupt
#define INT1ISON     (INTCON3 & 0b00001000)  != 0 // Bit 3 != 0 Int 1 external interrupt is enabled
#define INT1ISOFF    (INTCON3 & 0b00001000)  == 0 // Bit 3 == 0 Int 1 external interrupt is disabled
#define CLEARINT1IF  INTCON3 &= 0b11111110 // Bit 0 =  0 Int 1 external interrupt Flag
#define INT1IFISSET  (INTCON3 & 0b00000001) != 0 // Bit 0 != 0 Int 1 external interrupt is Set
#define INT1IFISCLEAR (INTCON3 & 0b00000001) == 0 // Bit 0 == 0 Int 1 external interrupt is Clear

// PIR2:                      PERIPHERAL INTERRUPT REQUEST (FLAG) REGISTER 2
// bit 7 OSCFIF:              Oscillator Fail Interrupt Flag bit
//                              1 = Device oscillator failed, clock input has
//                              changed to INTOSC (must be cleared in software)
//                              0 = Device clock operating
// bit 6 CMIF:                Comparator Interrupt Flag bit
//                              1 = Comparator input has changed (must be
//                              cleared in software) 0 = Comparator input has
//                              not changed
// bit 5 USBIF:               Solo per Pic con periferica Usb a bordo
//                              1 = Usb has requested an interrupt (must be
//                              cleared in software) 0 = No Usb interrupt
//                              request
// bit 4 EEIF:                Data EEPROM/Flash Write Operation Interrupt Flag
// bit
//                              1 = The write operation is complete (must be
//                              cleared in software) 0 = The write operation is
//                              not complete or has not been started
// bit 3 BCLIF:               Bus Collision Interrupt Flag bit
//                              1 = A bus collision occurred (must be cleared
//                              in software) 0 = No bus collision occurred
// bit 2 HLVDIF:              High/Low-Voltage Detect Interrupt Flag bit
//                              1 = A high/low-voltage condition occurred
//                              (direction determined by VDIRMAG bit,
//                              HLVDCON<7>) 0 = A high/low-voltage condition
//                              has not occurred
// bit 1 TMR3IF:              TMR3 Overflow Interrupt Flag bit
//                              1 = TMR3 register overflowed (must be cleared
//                              in software) 0 = TMR3 register did not overflow
// bit 0 CCP2IF:              CCP2 Interrupt Flag bit
//                            Capture mode:
//                              1 = A TMR1 register capture occurred (must be
//                              cleared in software) 0 = No TMR1 register
//                              capture occurred
//                            Compare mode:
//                              1 = A TMR1 register compare match occurred
//                              (must be cleared in software) 0 = No TMR1
//                              register compare match occurred
//                            PWM mode:
//                              Unused in this mode.
//
#define CLEAREEPIF   PIR2 &= 0b11101111 // Bit 4 (EEIF) = 0 Clear Flag Write Eeprom End

#define CLEARSSPCOLLISIONIF PIR2 &= 0b11110111 // Bit 3 (BCLIF) = 0 Clear Flag collision da SSP (I2C)
#define SSPCOLLISIONIFISSET (PIR2 & 0b00001000) != 0
#define SSPCOLLISIONIFISCLEAR (PIR2 & 0b00001000) == 0

#define CLEARTIMER3IF PIR2 &= 0b11111101 // Bit 1 (TMR3IF) = 0 Reset del flag di interrupt di
                     // TIMER3
#define TIMER3IFISSET (PIR2 & 0b00000010)  != 0 // Bit 1 (TMR3IF) != 0 Flag overflow interrupt di Timer 3 is set
#define TIMER3IFISCLEAR (PIR2 & 0b00000010)  == 0 // Bit 1 (TMR3IF) == 0 Flag interrupt timer 2 overflow == 0

#define CLEARCCP2IF  PIR2 &= 0b11111110 // Bit 0 (CCP2IF) == 0 Ccp2 interrupt Flag Clear
#define CCP2IFISSET  (PIR2 & 0b00000001) != 0 // Bit 0 (CCP2IF)  =   Ccp2 interrupt Flag is set
#define CCP2IFISCLEAR (PIR2 & 0b00000001) == 0 // Bit 0 (CCP2IF)  =   Ccp2 interrupt Flag not set
#endif

// T1CON:                       TIMER1 CONTROL REGISTER
// bit 7 RD16:                  16-bit Read/Write Mode Enable bit
//                                1 = Enables register Read/Write of Timer1 in
//                                one 16-bit op 0 = Enables register Read/Write
//                                of Timer1 in two 8-bit op
// bit 6 T1RUN:                 Timer1 System Clock Status bit
//                                1 = Device clock is derived from Timer1
//                                oscillator 0 = Device clock is derived from
//                                another source
// bit 5-4 T1CKPS1:             T1CKPS0: Timer1 Input Clock Prescale Select
// bits
//                                11 = 1:8 Prescale value
//                                10 = 1:4 Prescale value
//                                01 = 1:2 Prescale value
//                                00 = 1:1 Prescale value
// bit 3 T1OSCEN:               Timer1 Oscillator Enable bit
//                                1 = Timer1 Oscillator is enabled
//                                0 = Timer1 Oscillator is shut-off
//                                The oscillator inverter and feedback resistor
//                                are turned off
//                                  to eliminate power drain.
// bit 2 T1SYNC:                Timer1 External Clock Input Synchronization
// Select bit
//                              When TMR1CS = 1:
//                                1 = Do not synchronize external clock input
//                                0 = Synchronize external clock input
//                              When TMR1CS = 0:
//                                This bit is ignored. Timer1 uses the internal
//                                clock when TMR1CS = 0.
// bit 1 TMR1CS:                Timer1 Clock Source Select bit
//                                1 = External clock from pin RC0/T1OSO/T13CKI
//                                (on the rising edge) 0 = Internal clock
//                                (FOSC/4)
// bit 0 TMR1ON:                Timer1 On bit
//                                1 = Enables Timer1
//                                0 = Stops Timer1

#define TIMER1_16BIT T1CON |= 0b10000000 // Bit 7 (RD16)  = 1 Enables Timer1 1 * 16 bit register
                      // Read/Write
#define TIMER1_8BIT  T1CON &= 0b01111111 // Bit 7 (RD16)  = 0 Enables Timer1 2 *  8 bit register
                      // Read/Write
#define CLOCKSECONDARYISON (T1CON & 0b01000000) != 0 // Bit 6 (T1RUN) = 1 Device clock is derived from
                              // Timer1 oscillator (sola lettura)
#define CLOCKSECONDARYISOFF (T1CON & 0b01000000)  == 0 // Bit 6 (T1RUN) = 0 Device clock is derived from other source
#define TIMER1PRE008 T1CON |= 0b00110000 // Bit 5-4 = 11 prescale value 1:8
#define TIMER1PRE004 T1CON |= 0b00100000 // Bit 5-4 = 10 prescale value 1:4
#define TIMER1PRE002 T1CON |= 0b00010000 // Bit 5-4 = 01 prescale value 1:2
#define TIMER1PRERESET T1CON &= 0b11001111 // Bit 5-4 = 00 prescale value 1:1 (Reset prescaler
                      // value)
#define TIMER1EXTOSCON T1CON |= 0b00001000 // Bit   3 (T1OSCEN) = 1 Timer1 External Osc Enabled
                      // (Settare pin In prima di abilitare oscillatore)
#define TIMER1EXTOSCOFF T1CON &= 0b11110111 // Bit   3 (T1OSCEN) = 0 Timer1 External Osc Disabled
                      // (Settare pin In prima di abilitare oscillatore)
#define TIMER1SYNCON T1CON &= 0b11111011 // Bit   2 (T1SYNC)  = 0 External clock     sync (only
                      // when TMR1CS = 1)
#define TIMER1SYNCOFF T1CON |= 0b00000100 // Bit   2 (T1SYNC)  = 1 External clock not sync (only
                      // when TMR1CS = 1)
#define TIMER1CLOCKSOURCEEXT T1CON |= 0b00000010 // Bit   1 (TMR1CS)  = 1 Timer1Clock = External clock
                      // from pin RC0/T1OSO/T13CKI (on the rising edge)
#define TIMER1CLOCKSOURCEINT T1CON &= 0b11111101 // Bit   1 (TMR1CS)  = 0 Timer1Clock = Internal clock
                      // (FOSC/4)
#define TIMER1ON T1CON |= 0b00000001 // Bit   0 (TMR1ON)  = 1 abilita Timer 1
#define TIMER1OFF    T1CON &= 0b11111110 // Bit   0 (TMR1ON)  = 0 disabilita Timer 1
#define TIMER1ISON   (T1CON & 0b00000001) != 0 // Bit   0 (TMR1ON)  = 1 Timer 1 e' attivo
#define TIMER1ISOFF  (T1CON & 0b00000001) == 0 // Bit   0 (TMR1ON)  = 0 Timer 1 non e' attivo

#define DIRTIMER1EXTOSC TRISC
#define TIMER1EXTOSCBIT PORTC
#define TIMER1EXTOSCLAT LATC

#define DIRTIMER1EXTOSCIN DIRTIMER1EXTOSC |= 0b00000011 // Rc 1-0 Input Oscill Timer1
#define DIRTIMER1EXTOSCOUT DIRTIMER1EXTOSC &= 0b11111100

#define TIMER1EXTOSCHIGH TIMER1EXTOSCLAT |= 0b00000011 // Rc 1-0 = High
#define TIMER1EXTOSCLOW TIMER1EXTOSCLAT &= 0b11111100  // Rc 1-0 = Low

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// OSCTUNE:   OSCILLATOR TUNING REGISTER // bit 7      INTSRC: Internal
// Oscillator Low-Frequency Source Select bit     //
//            1 = 31.25 kHz device clock derived from 8 MHz INTOSC source //
//                          (divide-by-256 enabled) //
//            0 = 31 kHz device clock derived directly from INTRC internal //
//                          oscillator //
// bit 6      PLLEN: Frequency Multiplier PLL for INTOSC Enable bit(1) //
//            1 = PLL enabled for INTOSC (4 MHz and 8 MHz only) // 0 = PLL
//            disabled                                                //
// bit 5      Unimplemented: Read as .0. // bit 4-0    TUN4:TUN0: Frequency
// Tuning bits                                //
//            011111 = Maximum frequency // . . // . . // 000001 // 000000 =
//            Center frequency. Oscillator module is running at the  //
//                     calibrated frequency. //
//            111111 // . . // . . // 100000 = Minimum frequency //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
// RCON REGISTER                Note 1: If SBOREN is enabled, its Reset state
// is �1�;
//                              otherwise, it is �0�. See Register 4-1 for
//                              additional information. If the BOREN
//                              configuration bit is set (Brown-out Reset
//                              enabled), the BOR bit is Â1Â on a Power-on
//                              Reset. After a Brownout Reset has occurred, the
//                              BOR bit will be cleared, and must be set by
//                              firmware to indicate the occurrence of the next
//                              Brown-out Reset. Note 2: It is recommended that
//                              the POR bit be set after a Power-on Reset has
//                              been detected, so that subsequent Power-on
//                              Resets may be detected.
// bit 7    IPEN:               Interrupt Priority Enable bit
//                                1 = Enable priority levels on interrupts
//                                0 = Disable priority levels on interrupts
//                                (16CXXX
//                                    Compatibility mode)
// bit 6-5 Unimplemented:       Read as '0'
// bit 4    RI:                 RESET Instruction Flag bit
//                                1 = The RESET instruction was not executed
//                                0 = The RESET instruction was executed
//                                causing a device RESET (must be set in
//                                software after a Brown-out Reset occurs)
// bit 3    TO:                 Watchdog Time-out Flag bit
//                                1 = After power-up, CLRWDT instruction, or
//                                SLEEP instruction 0 = A WDT time-out occurred
// bit 2    PD:                 Power-down Detection Flag bit
//                                1 = After power-up or by the CLRWDT
//                                instruction 0 = By execution of the SLEEP
//                                instruction
// bit 1    POR:                Power-on Reset Status bit
//                                1 = A Power-on Reset has not occurred
//                                0 = A Power-on Reset occurred
//                                (must be set in software after a Power-on
//                                Reset occurs)
// bit 0    BOR:                Brown-out Reset Status bit
//                                1 = A Brown-out Reset has not occurred
//                                0 = A Brown-out Reset occurred
//                                (must be set in software after a Brown-out
//                                Reset occurs)
//
#define PRIORITYINTON RCON |= 0b10000000 // Bit 7 (IPEN) = 1 abilita interrupt priority
#define PRIORITYINTOFF RCON &= 0b01111111 // Bit 7 (IPEN) = 0 disabilita interrupt priority (16CXX
                     // Compatibility mode)

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// UCFG: USB CONFIGURATION REGISTER //
//                                                                            //
// bit 7 UTEYE:   USB Eye Pattern Test Enable bit //
//                1 = Eye pattern test enabled // 0 = Eye pattern test disabled
//                //
// bit 6 UOEMON:  USB OE Monitor Enable bit(1) //
//                1 = UOE signal active; it indicates intervals during which //
//                    the D+/D- lines are driving //
//                0 = UOE signal inactive //
// bit 5 Unimplemented: Read as �0� // bit 4 UPUEN:   USB On-Chip Pull-up
// Enable bit(2,3)                         //
//                1 = On-chip pull-up enabled (pull-up on D+ with FSEN = 1 //
//                    or D- with FSEN = 0) //
//                0 = On-chip pull-up disabled //
// bit 3 UTRDIS:  On-Chip Transceiver Disable bit(2) //
//                1 = On-chip transceiver disabled; digital transceiver //
//                    interface enabled //
//                0 = On-chip transceiver active //
// bit 2 FSEN:    Full-Speed Enable bit(2) //
//                1 = Full-speed device: controls transceiver edge rates; //
//                    requires input clock at 48 MHz //
//                0 = Low-speed device: controls transceiver edge rates; //
//                    requires input clock at 6 MHz //
// bit 1-0 PPB1:PPB0: Ping-Pong Buffers Configuration bits //
//                11 = Even/Odd ping-pong buffers enabled for EP 1 to 15 // 10
//                = Even/Odd ping-pong buffers enabled for all endpoints   //
//                01 = Even/Odd ping-pong buffer enabled for OUT Endpoint 0 //
//                00 = Even/Odd ping-pong buffers disabled //
//                                                                            //
// Note 1: If UTRDIS is set, the UOE signal will be active independent of the
// // UOEMON bit setting. // Note 2: The UPUEN, UTRDIS and FSEN bits should
// never be changed while the  // USB module is enabled. These values must be
// preconfigured prior to enabling// the module. // Note 3: This bit is only
// valid when the on-chip transceiver is active      // (UTRDIS = 0);
// otherwise, it is ignored.                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#define USBPULLUPON  UCFG |= 0b00010000 // bit 4 (UPUEN) = 1 USB On-Chip Pull-up Abilitate
#define USBPULLUPOFF UCFG &= 0b11101111 // bit 4 (UPUEN) = 0 USB On-Chip Pull-up Disabilitate

#define USBFSENHIGHSPEED UCFG |= 0b00000100 // bit 2 (FSEN) = 1 USB On-Chip FULL SPEED
#define USBFSENLOWSPEED UCFG &= 0b11111011 // bit 2 (FSEN) = 0 USB On-Chip LOW  SPEED

#define USBTRANSCEIVEROFF UCFG |= 0b00001000 // bit 3 (UTRDIS) = 1 = On-chip transceiver disabled
#define USBTRANSCEIVERON UCFG &= 0b11110111 // bit 3 (UTRDIS) = 0 = On-chip transceiver enabled

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// UCON: USB CONTROL REGISTER // bit 7 Unimplemented: Read as �0� // bit 6
// PPBRST:  Ping-Pong Buffers Reset bit                                 //
//                1 = Reset all Ping-Pong Buffer Pointers to the Even Buffer //
//                    Descriptor (BD) banks //
//                0 = Ping-Pong Buffer Pointers not being reset //
// bit 5 SE0: Live Single-Ended Zero Flag bit //
//                1 = Single-ended zero active on the USB bus // 0 = No
//                single-ended zero detected                           //
// bit 4 PKTDIS: Packet Transfer Disable bit //
//                1 = SIE token and packet processing disabled, automatically
//                //
//                    set when a SETUP token is received //
//                0 = SIE token and packet processing enabled //
// bit 3 USBEN: USB Module Enable bit //
//                1 = USB module and supporting circuitry enabled //
//                    (device attached) //
//                0 = USB module and supporting circuitry disabled //
//                    (device detached) //
// bit 2 RESUME: Resume Signaling Enable bit //
//                1 = Resume signaling activated // 0 = Resume signaling
//                disabled                               //
// bit 1 SUSPND: Suspend USB bit //
//                1 = USB module and supporting circuitry in Power Conserve //
//                    mode, SIE clock inactive //
//                0 = USB module and supporting circuitry in normal //
//                    operation, SIE clock clocked at the configured rate //
// bit 0 Unimplemented: Read as �0� //
//                                                                            //
// Note: When disabling the USB module, make sure the SUSPND bit (UCON<1>) //
// is clear prior to clearing the USBEN bit. // Clearing the USBEN bit when the
// module is in the suspended state may prevent/ the module from fully powering
// down.                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

#define USBPERIFOFF  UCON &= 0b11110111 // Bit 4 (PKTDIS) = 1 = SIE token and packet processing
                     // disabled (automatically set when a SETUP token is
                     // received)
#define USBPERIFON   UCON |= 0b00001000 // Bit 4 (PKTDIS) = 0 = SIE token and packet processing
                     // enabled

#define USBMODULEOFF UCON &= 0b11110111 // Bit 3 (USBEN) = 0 USB module Off and support circuitry
                     // disabled (device detached)
#define USBMODULEON  UCON |= 0b00001000 // Bit 3 == 1 USB module Off and support circuitry enabled
                     // (device attached)

#define SUSPENDUSBON UCON |= 0b00000010 // Bit 1 (SUSPND) = 1 = USB module and supporting
                     // circuitry in Power Conserve mode, SIE clock inactive
#define SUSPENDUSBOFF UCON &= 0b11111101 // Bit 1 (SUSPND) = 0 = USB module and supporting
                     // circuitry in Normal Operation mode, SIE clock active

#define PAUSAHIGHFALL 0x10 // Settare Int per Falling Edge
#define PAUSALOWRIS 0x01   // Settare Int per Rising  Edge

// Capture/Compare Module
#define DIRCCP1CTRLIN TRISC |= 0b00000100  // TrisC2 = 1
#define DIRCCP1CTRLOUT TRISC &= 0b11111011 // TrisC2 = 0

#define CCP1HIGH LATC |= 0b00000100 // Lat C 2
#define CCP1LOW LATC &= 0b11111011
#define CCP1ISHIGH (PORTC & 0b00000100) != 0 // Port C 2
#define CCP1ISLOW (PORTC & 0b00000100) == 0

#define DIRCCP2CTRLIN TRISC |= 0b00000010      // TrisC1 = 1
#define DIRCCP2CTRLOUT TRISC &= 0b11111101     // TrisC1 = 0
#define CCP2HIGH LATC |= 0b00000010            // LatC1 = 1
#define CCP2LOW LATC &= 0b11111101             // LatC1 = 0
#define CCP2ISHIGH (PORTC & 0b00000010) != 0 // portC1 != 0
#define CCP2ISLOW (PORTC & 0b00000010) == 0  // portC1 == 0

#define DIRALLCCPOUT TRISC &= 0b11111001; // Setta output (disabilita) TrisC2 (Ccp1) e TRISC1
                       // (Ccp2)
#define DIRALLCCPIN  TRISC |= 0b00000110; // Setta input     (abilita) TrisC2 (Ccp1) e TRISC1
                       // (Ccp2)

#define PAUSAHIGHFALL 0x10 // Settare Int per Falling Edge
#define PAUSALOWRIS 0x01   // Settare Int per Rising  Edge

#define INITIPR1 IPR1 = 0xff
#define INITFLAGIPIR1 PIR1 = 0
#define INITIPR2 IPR2 = 0xff
#define INITFLAGIPIR2 PIR2 = 0

#if defined(_PIC12F1840_H_)
//
// OSCCON:      OSCILLATOR CONTROL REGISTER
// bit 7        SPLLEN: Software PLL Enable bit
//              If PLLEN in Configuration Words = 1:
//              SPLLEN bit is ignored. 4x PLL is always enabled (subject to
//              oscillator requirements) If PLLEN in Configuration Words = 0:
//                        1 = 4x PLL Is enabled
//                        0 = 4x PLL is disabled
// bit 6-3 IRCF<3:0>: Internal Oscillator Frequency Select bits
//                        1111 = 16 MHz HF
//                        1110 = 8 MHz or 32 MHz HF(see Section 5.2.2.1
//                        �HFINTOSC�) 1101 = 4MHz HF 1100 = 2MHz HF 1011 = 1MHz
//                        HF 1010 = 500 kHz HF(1) 1001 = 250 kHz HF(1) 1000 =
//                        125 kHz HF(1) 0111 = 500 kHz MF (default upon Reset)
//                        0110 = 250 kHz MF
//                        0101 = 125 kHz MF
//                        0100 = 62.5 kHz MF
//                        0011 = 31.25 kHz HF(1)
//                        0010 = 31.25 kHz MF
//                        000x = 31kHz LF
// bit 2        Unimplemented: Read as �0�
// bit 1-0      SCS<1:0>: System Clock Select bits
//                        1x = Internal oscillator block
//                        01 = Timer1 oscillator
//                        00 = Clock determined by FOSC<2:0> in Configuration
//                        Words.
// Note 1: Duplicate frequency derived from HFINTOSC.
//
// Set Pll Internal Source Oscillator (non presente su 24/44-50/55
//
#define CLKPLLON OSCCON |= 0b10000000
#define CLKPLLOFF OSCCON &= 0b01111111

#define INTOSCCLEAR  OSCCON &= 0b10000111 // OscCon bit 6 to 4 set internal oscillator clock speed
                       // RESET
#define INTOSC016M OSCCON |= 0b01111000
#define INTOSC008M OSCCON |= 0b01110000 // 8 MHz or 32 MHz HF(see Section 5.2.2.1 �HFINTOSC�)
#define INTOSC004M OSCCON |= 0b01101000
#define INTOSC002M OSCCON |= 0b01100000
#define INTOSC001M OSCCON |= 0b01011000
#define INTOSC500K OSCCON |= 0b01010000
#define INTOSC250K OSCCON |= 0b01001000
#define INTOSC125K OSCCON |= 0b01000000
#define INTOSC0625K OSCCON |= 0b00100000
#define INTOSC031KHF OSCCON |= 0b00011000
#define INTOSC031KMF OSCCON |= 0b00010000
#define INTOSC031KLF OSCCON |= 0b00000000
#define INTERNALOSC  OSCCON |= 0b00000010 // Bit 1-0 == 1 - 0 internal oscillator
#define SECONDARYOSC OSCCON |= 0b00000001 // Bit 1-0 == 0 - 1 Secondary (Timer1) oscillator
#define PRIMARYOSC   OSCCON &= 0b11111100 // Bit 1-0 OSCCON == 00 Select primary oscillator (Reset
                       // register)
#else
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// OSCCON:  OSCILLATOR CONTROL REGISTER // bit 7    IDLEN: Idle Enable bit //
//          1 = Device enters Idle mode on SLEEP instruction // 0 = Device
//          enters Sleep mode on SLEEP instruction                 //
// bit 6-4  IRCF2:IRCF0: Internal Oscillator Frequency Select bits //
//          111 = 8 MHz (INTOSC drives clock directly) // 110 = 4 MHz // 101 =
//          2 MHz                                                       // 100
//          = 1 MHz(3)                                                    //
//          011 = 500 kHz // 010 = 250 kHz // 001 = 125 kHz // 000 = 31 kHz
//          (from either INTOSC/256 or INTRC directly)(2)        //
// bit 3    OSTS: Oscillator Start-up Timer Time-out Status bit(1) //
//          1 = Oscillator Start-up Timer (OST) time-out has expired; //
//                                      primary oscillator is running //
//          0 = Oscillator Start-up Timer (OST) time-out is running; //
//                                      primary oscillator is not ready //
// bit 2    IOFS: INTOSC Frequency Stable bit //
//          1 = INTOSC frequency is stable // 0 = INTOSC frequency is not
//          stable                                //
// bit 1-0  SCS1:SCS0: System Clock Select bits //
//          1x = Internal oscillator block // 01 = Secondary (Timer1)
//          oscillator                                // 00 = Primary
//          oscillator                                           //
//                                                                            //
// The Timer1 oscillator should already be running prior to entering // SEC_RUN
// mode. If the T1OSCEN bit is not set when the SCS1:SCS0 bits are    // set to
// .01., entry to SEC_RUN mode will not occur. If the Timer1           //
// oscillator is enabled, but not yet running, device clocks will be delayed //
// until the oscillator has started. In such situations, initial oscillator //
// operation is far from stable and unpredictable operation may result. //
//                                                                            //
// Note 1:    Reset state depends on state of the IESO Configuration bit. //
// Note 2:    Source selected by the INTSRC bit (OSCTUNE<7>), see text. // Note
// 3:    Default output frequency of INTOSC on Reset.                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

// Set Low Frequency Internal Source Oscillator (WatchDog e Secondary Clock per
// Rtc)
#define LFINTERNALCLKSOURCEXTAL OSCTUNE |= 0b10000000 // Bit 7 (INTSRC) = 1 31.25 kHz device clock derived
                        // from 8 MHz INTOSC source
#define LFINTERNALCLKSOURCEINTERNAL         OSCTUNE &= 0b01111111 // Bit 7 (INTSRC) = 0 31 kHz device clock derived
                        // directly from INTRC internal

// Set Pll Internal Source Oscillator (non presente su micro con periferica USB
// a bordo
#define CLKPLLON OSCTUNE |= 0b01000000 // Bit 6 (PLLEN)  = 1   Enable Clock Pll
#define CLKPLLOFF    OSCTUNE &= 0b10111111 // Bit 6 (PLLEN)  = 0   Disable Clock Pll
#endif
#define SLEEPISIDLE  OSCCON |= 0b10000000 // Bit 7 (IDLEN)  = 1   Sleep enter Sleep Mode
#define SLEEPISSLEEP OSCCON &= 0b01111111 // Bit 7 (IDLEN)  = 0   Sleep enter idle  Mode
#define INTOSC008M OSCCON |= 0b01110000 // Bit 6-4 (IRCF) = 111
#define INTOSC004M OSCCON |= 0b01100000 // Bit 6-4 (IRCF) = 110
#define INTOSC002M OSCCON |= 0b01010000 // Bit 6-4 (IRCF) = 101
#define INTOSC001M OSCCON |= 0b01000000 // Bit 6-4 (IRCF) = 100
#define INTOSC500K OSCCON |= 0b00110000 // Bit 6-4 (IRCF) = 011
#define INTOSC250K OSCCON |= 0b00100000 // Bit 6-4 (IRCF) = 010
#define INTOSC125K OSCCON |= 0b00010000 // Bit 6-4 (IRCF) = 001
#define INTOSC031K   OSCCON &= 0b10001111 // Bit 6-4 (IRCF) = 000 Reset internal oscillator clock
                       // speed
#define CLKPRIMARYISNOTSTABLE (OSCCON & 0b00001000)     == 0 // Bit 3 (OSTS)  == 0   Oscillator Start-up Timer (OSTS) time-out is
           // running; primary oscillator is not ready
#define CLKINTERNALISNOTSTABLE (OSCCON & 0b00000100) == 0 // Bit 2 (IOFS)  == 0   INTOSC Internal Oscillator
                             // Frequency is not stable
#define INTERNALOSC  OSCCON |= 0b00000010 // Bit 1-0 (SCS)  = 1-0 Internal oscillator
#define SECONDARYOSC OSCCON |= 0b00000001 // Bit 1-0 (SCS)  = 0-1 Secondary (Timer1) oscillator
#define PRIMARYOSC   OSCCON &= 0b11111100 // Bit 1-0 (SCS)  = 0-0 Primary oscillator (Reset
                       // register)

/*! \brief Settings per oscillatore USB
 *
 *  XTAL      4 MHz.
 *  XTPLL_XT
 *  PLLDIV    2
 *  CPUDIV    2     Oscillatore cpu 96 / 6 = 16 MHz.  (0)
 *  USBDIV    2     48 MHz USB generati direttamente
 *  FSEN      1     Abilitazione USB HI Speed
 *
 */

//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  Fine file //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//

//
// E o f
//
