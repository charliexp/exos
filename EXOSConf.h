/*************************************************************************\
 * File Name	: EXOSConf.h			     	         	 *
 * --------------------------------------------------------------------- *
 * Title	: EXOS --- Cooperative system scheduler                  *
 * Revision	: V1.0                                                   *
 * Notes	:                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When	      Who	  Revision	 Description of change   *
 * -----------	  -----------	 ---------      ------------------------ *
 * 15-10-2010	  charlie_weng	   V1.0	         Created the program     *
 *                                                                       * 
\*************************************************************************/

#include "stm8s.h"

// stm8s define
//////////////////////////////////////////////////////////////////////////

#define  ISR_EN()                   enableInterrupts()
#define  ISR_DI()                   disableInterrupts()

#define  STK                        @near int

//////////////////////////////////////////////////////////////////////////

#define MAX_THREAD_NUM              4  
//////////////////////////////////////////////////////////////////////////

#define GET_STACK(p)    _asm("ldw Y, SP \n ldw (X), Y",p);  //GET_STACK(int* p)
			
#define	SET_STACK(p)    _asm("ldw X,(X) \n ldw SP,X",p);    //SET_STACK(int *p)
 
//////////////////////////////////////////////////////////////////////////
