/*
 * APP.h
 *
 * Created: 9/14/2021 6:53:10 PM
 *  Author: Mohamed El-Kholy
 */ 


#ifndef APP_H_
#define APP_H_


/* Prototype of the Function */
void APP_Init(void);
void APP_update(void);
void Interrupt (void);
u8 RandomNum (u8 copy_u8MinValue , u8 copy_u8MaxValue , u8 copy_u8IteratorNum);


#define		USER_ENTERED_STATE			1
#define		USER_NOT_ENTERED_STATE		0
#define		ALLOWED_INPUT_RANGE			4
#define		ALLOWED_SECONDS_DELAY		3	


#endif /* APP_H_ */