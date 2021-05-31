/**
  CMP2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.c

  @Summary
    This is the generated driver implementation file for the CMP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for CMP2.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CK256MP506
        Driver Version    :  1.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.45
*/


/**
  Section: Included Files
*/

#include "cmp2.h"

/**
  Section: CMP2 APIs
*****************************************************************************************/
/******************************************************************************
*                                                                             
*    Function:			CMP2_Initialize
*    Description:       Initialization of the Comparator with Slope Compensation module                                                                         
*                                             
*    Return Value:      None
******************************************************************************/

void CMP2_Initialize(void)
{           

    // Disable the CMP module before the initialization
    CMP2_Disable();
	
	
	// Comparator Register settings
	DACCTRL1L = 0x00; //CLKDIV 1:1; DACON disabled; DACSIDL disabled; FCLKDIV 1:1; CLKSEL AFVCO/2 - Auxiliary VCO Clock; 
	DACCTRL2L = 0x55; //TMODTIME 85; 
	DACCTRL2H = 0x8A; //SSTIME 138; 
	DAC2CONH = 0x00; //TMCB 0; 
	DAC2CONL = 0x8208; //CMPPOL Non Inverted; HYSPOL Rising Edge; HYSSEL None; DACEN enabled; FLTREN disabled; CBE disabled; IRQM Interrupts are disabled; INSEL CMP2B; DACOEN enabled; 

	//Slope Settings
	SLP2CONH = 0x00; //HME disabled; PSE Negative; SLOPEN disabled; TWME disabled; 
	SLP2CONL = 0x00; //HCFSEL None; SLPSTRT None; SLPSTOPB None; SLPSTOPA None; 
	SLP2DAT = 0x00; //SLPDAT 0; 
	DAC2DATL = 0x00; //DACDATL 0; 
	DAC2DATH = 0x00; //DACDATH 0; 
    
	
    CMP2_Enable();

}

/******************************************************************************
*                                                                             
*    Function:			CMP2_ComparatorOuputStatusGet
*    Description:       Returns the status of the Comparator including the 
*						polarity                                                                       
*                                             
*    Return Value:      True if the Comparator Status is 1 and False if it is 0
******************************************************************************/
bool CMP2_ComparatorOuputStatusGet(void)
{
    return (DAC2CONLbits.CMPSTAT);
}

/******************************************************************************
*                                                                             
*    Function:			CMP2_Enable
*    Description:       Enables the common DAC module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
void CMP2_Enable(void)
{
    DACCTRL1Lbits.DACON = 1;
}

/******************************************************************************
*                                                                             
*    Function:			CMP2_Disable
*    Description:       Disables the common DAC module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
void CMP2_Disable(void)
{
    DACCTRL1Lbits.DACON = 0;
}

/******************************************************************************
*                                                                             
*    Function:			CMP2_SetInputSource
*    Description:       Set the source to the non-inverting input of the 
*						Comparator module                                                                     
*      
*	 Parameters:		Enumeration specifying the input source                                      
*    Return Value:      None
******************************************************************************/
void CMP2_SetInputSource(CMP2_INPUT inpSrc)
{
    DAC2CONLbits.INSEL = inpSrc;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetDACDataHighValue
*    Description:       Sets the DAC Data High Value                                     
*      
*    Parameters:        Value to be set as DAC Data High                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetDACDataHighValue(uint16_t dacVal)
{
    DAC2DATH = dacVal;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetDACDataLowValue
*    Description:       Sets the DAC Data Low Value                                     
*      
*    Parameters:        Value to be set as DAC Data Low                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetDACDataLowValue(uint16_t dacVal)
{
    DAC2DATL = dacVal;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_EnableDACOutput
*    Description:       Enables the DAC Output                                    
*      
*    Parameters:        None                                       
*    Return Value:      None
******************************************************************************/
void CMP2_EnableDACOutput(void)
{
    DAC2CONLbits.DACOEN = 1;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_DisableDACOutput
*    Description:       Disables the DAC Output                                    
*      
*    Parameters:        None                                       
*    Return Value:      None
******************************************************************************/
void CMP2_DisableDACOutput(void)
{
    DAC2CONLbits.DACOEN = 0;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetStartTrigger
*    Description:       Trigger set for the Start Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetStartTrigger(CMP2_START_TRIGGER trigger)
{
    SLP2CONLbits.SLPSTRT = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetStopATrigger
*    Description:       Trigger set for the Stop A Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetStopATrigger(CMP2_STOPA_TRIGGER trigger)
{
    SLP2CONLbits.SLPSTOPA = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetStopBTrigger
*    Description:       Trigger set for the Stop B Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetStopBTrigger(CMP2_STOPB_TRIGGER trigger)
{
    SLP2CONLbits.SLPSTOPB = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP2_SetHystereticTrigger
*    Description:       Trigger set for the Hysteretic mode                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP2_SetHystereticTrigger(CMP2_HYSTERETIC_FUNCTION trigger)
{
    SLP2CONLbits.HCFSEL = trigger;
}

/* Callback function for the CMP2 module */
void __attribute__ ((weak)) CMP2_CallBack(void)
{
    // Add your custom callback code here
}

/******************************************************************************
*    Function:			CMP2_Tasks
*    Description:       The Task function can be called in the main application 
*						using the High Speed Comparator, when interrupts are not 
*						used.  This would thus introduce the polling mode feature 
*						of the Analog Comparator.                                                                     
*      
*	 Parameters:		None                                      
*    Return Value:      None 
******************************************************************************/
void CMP2_Tasks(void)
{
	if(IFS4bits.CMP2IF)
	{
		// CMP2 callback function 
		CMP2_CallBack();
	
		// clear the CMP2 interrupt flag
		IFS4bits.CMP2IF = 0;
	}
}

/**
 End of File
*/