/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     vmc_dcdt.h
  Project Name:  vmc
  Date:          5/18/2021
  Time:          16:20.12

  Software License Agreement

  Copyright © 2021 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _VMC_DCDT_DEFINES_H
#define _VMC_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 5.0000e+03 Hz
  Pole 2                 1.7500e+05 Hz
  Zero 1                 3.0000e+02 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          3.5000e+05
  PWM Sampling Ratio     8
  Sampling Frequency     4.3750e+04
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    2600
  Kuc Gain               6.5785e+02
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    16.480
  Gain                  1.094e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  3.14e+04   1.10e+06(1.88e+03 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       1.88e+03(1.10e+06 + s)



  Digital Compensator Coefficients

  Name    Value      Normalized    Q15      Hex
  ----    -----      ----------    ---      ---
  a1      0.147      0.009         306      0x0132
  a2      0.853      0.054         1771     0x06EB
  b0      15.771     1.000         32764    0x7FFC
  b1      0.665      0.042         1381     0x0565
  b2      -15.106    -0.958        -31382   0x856A


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (15.771) + (0.665)z^(-1) + (-15.106)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.147)z^(-1) - (0.853)z^(-2)

**/


// Compensator Coefficient Defines
#define VMC_COMP_2P2Z_COEFF_A1      0x0132
#define VMC_COMP_2P2Z_COEFF_A2      0x06EB
#define VMC_COMP_2P2Z_COEFF_B0      0x7FFC
#define VMC_COMP_2P2Z_COEFF_B1      0x0565
#define VMC_COMP_2P2Z_COEFF_B2      0x856A
#define VMC_COMP_2P2Z_POSTSCALER    0x7E2D
#define VMC_COMP_2P2Z_POSTSHIFT     0xFFFC
#define VMC_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define VMC_COMP_2P2Z_MIN_CLAMP    0x0000
#define VMC_COMP_2P2Z_MAX_CLAMP    0x0A28


#endif
