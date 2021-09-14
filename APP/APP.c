
/* Import the Drivers and Libraries */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/DIO/MDIO_interface.h"
#include "../EUCAL/KEYPAD/Keybad_interface.h"
#include "../EUCAL/LCD/lcd_interface.h"
#include "../MCAL/TIMER/Timer_interface.h"
#include "APP.h"
#include <stdlib.h>
#include <util/delay.h>

/* global counter holds number of Seconds */
u8 global_u8SecondsCounter = 0 ;
/* global Variable holds Random Number */
u8 global_u8RandomNum ;
/* global variable holds The Iterator Number */
u8 global_u8RandIterator = 0 ;
/* global Array holds the Keypad Input */
u8 global_u8KeypadInput [4] ;
/* global Variable holds the Conversion of keypad input String to Integer */
u8 global_u8InputNum = 0 ;
/* global Variable holds the state of the Entry user , If the user entered value holds 1 , if the user not entered value holds 0 */
u8 global_u8State ; 


/*********************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Initialize The Application
***********************************************************************/
void APP_Init(void)
{
		/* Initialize The LCD Screen */
	EULLCD_voidLCDInit();
	/* Initialize The Button */
	EUKeypad_voidKeypadInit();
	/* Write Guess The Number on the Screen */
	EULCD_voidLCDWriteString("Guess The Number");
	/* Go to Row 2 and Column 4 */
	EULCD_voidLCDGoto(2,8);
	/* Initialize The Timer ON 
	*	Normal Mode 
	*	Timer 1
	*	OverFLow Interrupt 
	*   1 MHZ Crystal 
	*	64  Prescaler		. */
	MTimer_voidInit();
	/* Make The Timer Interrupt Every 1 Second */
	MTimer_s8SetValue(49911);
	/* Set Call Back Function to be Executed on the ISR */
	MTimer_voidSetCallBack(Interrupt);
	/* Get Random Number and convert*/
	global_u8RandomNum = RandomNum(0,15,global_u8RandIterator) ;
	
}


/**********************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Update The Application
************************************************************************************/
void APP_update(void)
{
		/* local Variable holds the value return from Keypad */
		u8 local_u8ReturnKeypadValue ;
		/* local variable used in the iteration */
		u8 local_u8Iterator = 0;
		/* loop  */
		while (local_u8Iterator < ALLOWED_INPUT_RANGE)
		{
			/* Get The Input From the Keypad */
			local_u8ReturnKeypadValue = EUKeypad_voidKeyPressed() ;
			/* if the Keypad Pressed */
			if (local_u8ReturnKeypadValue != 0xFF)
			{
				/* Write the Input To The LCD */
				EULCD_voidLCDWriteData(local_u8ReturnKeypadValue);
				/* Store The Input in the Array */
				global_u8KeypadInput[local_u8Iterator]= local_u8ReturnKeypadValue ;
				/* if the User entered '=' break the while loop*/
				if (global_u8KeypadInput[local_u8Iterator] == '=')
				break ;
				/* increment the iterator */
				local_u8Iterator ++ ;		
			}
		}
		/* When User Enter "=" Call Validate Function */
		if (local_u8ReturnKeypadValue == '=')
		{
			Validate();
		}
}



/********************************************************************************************
* Parameters:(in) : NONE
* Parameters:(out): NONE
* Return value	  : NONE
* Description	  : Function that Call Every 1 Second and Execute the Condition every Call
**********************************************************************************************/
 void Interrupt(void)
{
	/* Increment The seconds */
	global_u8SecondsCounter ++ ;
	/* Check if the Seconds Reach to The Seconds Defined Macro */
	if (global_u8SecondsCounter == ALLOWED_SECONDS_DELAY )
	{	/* if the User Doesn't Entered ant Value */
		if (global_u8State == USER_NOT_ENTERED_STATE)
		{
			/* Inform the User That there is no input */
			NoInput();
		}
	}
		
}


/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Update The Screen After Every Guess
*******************************************************************************/
static void Screen_Update(void)
{
	/* Wait Some MILLI Seconds */
	_delay_ms(1000);
	/* Reset The Seconds Counter */
	global_u8SecondsCounter = 0 ;
	/* Reset The Iterator */
	global_u8RandIterator ++ ;
	/* Go to The First Position on the Screen */
	EULCD_voidLCDGoto(1,1);
	/* Write Guess The Number String */
	EULCD_voidLCDWriteString("Guess The Number");
	/* Generate Random Number of Range between 1 and 15 */
	global_u8RandomNum = RandomNum(0,15,global_u8RandIterator) ;
	/* Go To ROW 2 and Column 4*/
	EULCD_voidLCDGoto(2,8);
	/* Reset The State */
	global_u8State = USER_NOT_ENTERED_STATE ;
}

/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Check The Input and Inform the User if he Won or lose or Entered Invalid Range 
*******************************************************************************/

 void Validate(void)
{	
	/* Convert The State to User Enter Data */
	global_u8State = USER_ENTERED_STATE ; 
	/* Convert The Input String Input Data to integer */
	global_u8InputNum = atoi(global_u8KeypadInput);
	/* If the entered Range is valid */
	if ((global_u8InputNum < 15) && (global_u8InputNum !=0))
	{
		/* First Clear The Screen */
		EULCD_voidLCDClearScreen();
		/* Reset The Cursor */
		EULCD_voidLCDGoto(1,1);
		/* Write The Actual and Guess Value on the Screen */
		EULCD_voidLCDWriteString("Your Guess: ");
		EULCD_voidLCDDisplay_num(global_u8InputNum);
		EULCD_voidLCDGoto(2,1);
		EULCD_voidLCDWriteString("Actual    : ");
		EULCD_voidLCDDisplay_num(global_u8RandomNum);
		_delay_ms(1000);
		
		/* in case of the Guess is Correct */
		if (global_u8InputNum == global_u8RandomNum)
		{
			Winner();
			
		}
		/* in case of the guess is not correct */
		else if ( (global_u8InputNum != global_u8RandomNum) )
		{
			loser();

		}
	}

	/* in case of The Number of The Pressing is more than the Random Number Range */
	else
	{
		InvalidRange();
	}
			
	
}

/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Inform the User That he is Winner
*******************************************************************************/
 void Winner (void )
{
	/* Clear The Screen */
	EULCD_voidLCDClearScreen ();
	/* go to ROW 1 and Column 4 */
	EULCD_voidLCDGoto(1,4);
	/* Display " YOU WON !" */
	EULCD_voidLCDWriteString("YOU WON !");
	/* Update the Screen */
	Screen_Update();
}


/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Inform the User That he is Loser
*******************************************************************************/
 void loser (void)
{
	/* Clear The Screen */
	EULCD_voidLCDClearScreen ();
	/* go to ROW 1 and Column 4 */
	EULCD_voidLCDGoto(1,4);
	/* Display " YOU LOSE !" */
	EULCD_voidLCDWriteString("YOU LOSE !");
	/* Update the Screen */
	Screen_Update();
	
}


/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Inform the User That There's No Input Done 
*******************************************************************************/
 void NoInput (void)
{
	/* Clear The Screen */
	EULCD_voidLCDClearScreen ();
	/* go to ROW 1 and Column 1*/
	EULCD_voidLCDGoto(1,1);
	/* Inform the User  */
	EULCD_voidLCDWriteString("You Must Press");
	/* Update the Screen */
	Screen_Update();
}


/*****************************************************************************
* Parameters:(in) : None
* Parameters:(out): NONE
* Return value	  : None
* Description	  : Inform the User That he Entered Invalid Range
*******************************************************************************/
 void InvalidRange (void)
{
	/* Clear The Screen */
	EULCD_voidLCDClearScreen ();
	/* go to ROW 1 and Column 1*/
	EULCD_voidLCDGoto(1,1);
	/* Inform the User  */
	EULCD_voidLCDWriteString("Not Valid Range");
	/* Update the Screen */
	Screen_Update();
}

/**************************************************************************************
* Parameters:(in) : Minimum Value , Maximum Value , Number Of iteration
* Parameters:(out): NONE
* Return value	  : 8-bit unsigned char NONE
* Description	  : Generate Random Number Between Min and Max Valued
****************************************************************************************/

 u8 RandomNum (u8 copy_u8MinValue , u8 copy_u8MaxValue , u8 copy_u8IteratorNum)
{
	/* local Variable holds The Return */
	u8 local_u8Random ;
	for (copy_u8IteratorNum =0 ; copy_u8IteratorNum <=4 ; copy_u8IteratorNum ++ )
	{
		/* Generate Random Number Between Min and Max Valued */
		local_u8Random = rand() % ((copy_u8MaxValue+1)-copy_u8MinValue);
	}
	/* return the random number */
	return local_u8Random ;
}