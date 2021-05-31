/**
  CMP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp1.c

  @Summary
    This is the generated driver implementation file for the CMP1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for CMP1.
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

#include "cmp1.h"

/**
  Section: CMP1 APIs
*****************************************************************************************/
/******************************************************************************
*                                                                             
*    Function:			CMP1_Initialize
*    Description:       Initialization of the Comparator with Slope Compensation module                                                                         
*                                             
*    Return Value:      None
******************************************************************************/

void CMP1_Initialize(void)
{           

    // Disable the CMP module before the initialization
    CMP1_Disable();
	
	
	// Comparator Register settings
	DACCTRL1L = 0x87; //CLKDIV 1:1; DACON disabled; DACSIDL disabled; FCLKDIV 1:8; CLKSEL AFPLLO - Auxiliary Clock with PLL Enabled ; 
	DACCTRL2L = 0x55; //TMODTIME 85; 
	DACCTRL2H = 0x8A; //SSTIME 138; 
	DAC1CONH = 0x00; //TMCB 0; 
	DAC1CONL = 0x8108; //CMPPOL Non Inverted; HYSPOL Rising Edge; HYSSEL None; DACEN enabled; FLTREN enabled; CBE disabled; IRQM Interrupts are disabled; INSEL CMP1B; DACOEN disabled; 

	//Slope Settings
	SLP1CONH = 0x00; //HME disabled; PSE Negative; SLOPEN disabled; TWME disabled; 
	SLP1CONL = 0x00; //HCFSEL None; SLPSTRT None; SLPSTOPB None; SLPSTOPA None; 
	SLP1DAT = 0x00; //SLPDAT 0; 
	DAC1DATL = 0x00; //DACDATL 0; 
	DAC1DATH = 0x900; //DACDATH 2304; 
    
	
    CMP1_Enable();

}

/******************************************************************************
*                                                                             
*    Function:			CMP1_ComparatorOuputStatusGet
*    Description:       Returns the status of the Comparator including the 
*						polarity                                                                       
*                                             
*    Return Value:      True if the Comparator Status is 1 and False if it is 0
******************************************************************************/
bool CMP1_ComparatorOuputStatusGet(void)
{
    return (DAC1CONLbits.CMPSTAT);
}

/******************************************************************************
*                                                                             
*    Function:			CMP1_Enable
*    Description:       Enables the common DAC module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
void CMP1_Enable(void)
{
    DACCTRL1Lbits.DACON = 1;
}

/******************************************************************************
*                                                                             
*    Function:			CMP1_Disable
*    Description:       Disables the common DAC module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
void CMP1_Disable(void)
{
    DACCTRL1Lbits.DACON = 0;
}

/******************************************************************************
*                                                                             
*    Function:			CMP1_SetInputSource
*    Description:       Set the source to the non-inverting input of the 
*						Comparator module                                                                     
*      
*	 Parameters:		Enumeration specifying the input source                                      
*    Return Value:      None
******************************************************************************/
void CMP1_SetInputSource(CMP1_INPUT inpSrc)
{
    DAC1CONLbits.INSEL = inpSrc;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetDACDataHighValue
*    Description:       Sets the DAC Data High Value                                     
*      
*    Parameters:        Value to be set as DAC Data High                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetDACDataHighValue(uint16_t dacVal)
{
    DAC1DATH = dacVal;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetDACDataLowValue
*    Description:       Sets the DAC Data Low Value                                     
*      
*    Parameters:        Value to be set as DAC Data Low                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetDACDataLowValue(uint16_t dacVal)
{
    DAC1DATL = dacVal;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_EnableDACOutput
*    Description:       Enables the DAC Output                                    
*      
*    Parameters:        None                                       
*    Return Value:      None
******************************************************************************/
void CMP1_EnableDACOutput(void)
{
    DAC1CONLbits.DACOEN = 1;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_DisableDACOutput
*    Description:       Disables the DAC Output                                    
*      
*    Parameters:        None                                       
*    Return Value:      None
******************************************************************************/
void CMP1_DisableDACOutput(void)
{
    DAC1CONLbits.DACOEN = 0;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetStartTrigger
*    Description:       Trigger set for the Start Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetStartTrigger(CMP1_START_TRIGGER trigger)
{
    SLP1CONLbits.SLPSTRT = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetStopATrigger
*    Description:       Trigger set for the Stop A Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetStopATrigger(CMP1_STOPA_TRIGGER trigger)
{
    SLP1CONLbits.SLPSTOPA = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetStopBTrigger
*    Description:       Trigger set for the Stop B Signal of the Slope                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetStopBTrigger(CMP1_STOPB_TRIGGER trigger)
{
    SLP1CONLbits.SLPSTOPB = trigger;
}

/******************************************************************************
*                                                                             
*    Function:		CMP1_SetHystereticTrigger
*    Description:       Trigger set for the Hysteretic mode                                    
*      
*    Parameters:        Value indicating the trigger to be set                                       
*    Return Value:      None
******************************************************************************/
void CMP1_SetHystereticTrigger(CMP1_HYSTERETIC_FUNCTION trigger)
{
    SLP1CONLbits.HCFSEL = trigger;
}

/* Callback function for the CMP1 module */
void __attribute__ ((weak)) CMP1_CallBack(void)
{
    // Add your custom callback code here
}

/******************************************************************************
*    Function:			CMP1_Tasks
*    Description:       The Task function can be called in the main application 
*						using the High Speed Comparator, when interrupts are not 
*						used.  This would thus introduce the polling mode feature 
*						of the Analog Comparator.                                                                     
*      
*	 Parameters:		None                                      
*    Return Value:      None 
******************************************************************************/
void CMP1_Tasks(void)
{
	if(IFS4bits.CMP1IF)
	{
		// CMP1 callback function 
		CMP1_CallBack();
	
		// clear the CMP1 interrupt flag
		IFS4bits.CMP1IF = 0;
	}
}

/**
 End of File
*/