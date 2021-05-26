/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     avgcm_dcdt.h
  Project Name:  avgcm
  Date:          5/18/2021
  Time:          16:21.52

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
#ifndef _AVGCM_DCDT_DEFINES_H
#define _AVGCM_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 1.0000e+03 Hz
  Pole 2                 1.7500e+05 Hz
  Zero 1                 8.0000e+02 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          3.5000e+05
  PWM Sampling Ratio     2
  Sampling Frequency     1.7500e+05
  PWM Max Resolution     2.5000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       5.0000e-08
  Control Output Min.    0
  Control Output Max.    22842
  Kuc Gain               9.2098e+00
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    14.480
  Gain                  4.375e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  6.28e+03   1.10e+06(5.03e+03 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       5.03e+03(1.10e+06 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      0.483     0.483         15823    0x3DCF
  a2      0.517     0.517         16944    0x4230
  b0      0.962     0.962         31516    0x7B1C
  b1      0.027     0.027         892      0x037C
  b2      -0.935    -0.935        -30623   0x8861


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.962) + (0.027)z^(-1) + (-0.935)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.483)z^(-1) - (0.517)z^(-2)

**/


// Compensator Coefficient Defines
#define AVGCM_COMP_2P2Z_COEFF_A1      0x3DCF
#define AVGCM_COMP_2P2Z_COEFF_A2      0x4230
#define AVGCM_COMP_2P2Z_COEFF_B0      0x7B1C
#define AVGCM_COMP_2P2Z_COEFF_B1      0x037C
#define AVGCM_COMP_2P2Z_COEFF_B2      0x8861
#define AVGCM_COMP_2P2Z_POSTSCALER    0x7FFF
#define AVGCM_COMP_2P2Z_POSTSHIFT     0x0000
#define AVGCM_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define AVGCM_COMP_2P2Z_MIN_CLAMP    0x0000
#define AVGCM_COMP_2P2Z_MAX_CLAMP    0x593A


#endif
