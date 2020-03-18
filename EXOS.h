/*************************************************************************\
 * File Name	: EXOS.h			       		      	 *
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

#ifndef __EXOS_H__
#define __EXOS_H__

//////////////////////////////////////////////////////////////////////////

#include "EXOSConf.h"
#include "setjmp.h"

//////////////////////////////////////////////////////////////////////////

#define  EXOS_VERSION           "EXOS 1.0.0"

//////////////////////////////////////////////////////////////////////////

typedef  unsigned char  	    UCHAR;
typedef  unsigned char  	     BYTE;
typedef  unsigned short   	     WORD;
typedef  unsigned char  	     BOOL;
typedef  unsigned long              DWORD;
typedef  WORD                        TICK;
//////////////////////////////////////////////////////////////////////////
#define  UCHAR_MAX                    0xFF
#define  BYTE_MAX                     0xFF
#define  WORD_MAX                   0xFFFF
#define  DWORD_MAX              0xFFFFFFFF  
#define  TICK_MAX                 WORD_MAX

///////////////////////////////////////////////////////////////////////////
#ifndef  FALSE
 #define FALSE  			0 
#endif

#ifndef  TRUE
 #define TRUE   		   !FALSE
#endif

///////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL 				0
#endif 

///////////////////////////////////////////////////////////////////////////
#define assert(f)    do{ if(!f) return; }while(0)

#ifndef  ENTER_CRITICAL()
#define  ENTER_CRITICAL()   ISR_DI()
#endif

#ifndef  EXIT_CRITICAL() 
#define  EXIT_CRITICAL()    ISR_EN()
#endif

//////////////////////////////////////////////////////////////////////////
typedef struct Thread *Thread;

//////////////////////////////////////////////////////////////////////////
struct Thread
{
  	Thread    link;			// link point
  	Thread*   inqueue;		// Queue ind
  	
  	void (*thread)( void *arg );	// thread entry address
  	jmp_buf    mtcx;		// context 
        void       *arg; 		// arg
        TICK       tick;		// time tick
};

/////////////////////////////////////////////////////////////////////////
typedef Thread   HANDLE;

/////////////////////////////////////////////////////////////////////////
#define MTCX_SAVE(p)  		setjmp((p)->mtcx)

/////////////////////////////////////////////////////////////////////////
#define MTCX_RESTORE(p)		longjmp((p)->mtcx,1)

/////////////////////////////////////////////////////////////////////////
#define _swtch(p,q)      			\
	do{					\
	if (setjmp((p)->mtcx) == 0)    		\
		longjmp((q)->mtcx, 1);		\
	}while(0)		      
 
/////////////////////////////////////////////////////////////////////////      
#define EXOS_WAIT_UNTIL( condition )	        \
   do{						\
   	while(!(condition))			\
   	{					\
   		eXOSYield(); 		        \
   	}					\
     }while(0)     

/////////////////////////////////////////////////////////////////////////
#define EXOS_WAIT_WHILE(cond)   EXOS_WAIT_UNTIL(!(cond)) 

/////////////////////////////////////////////////////////////////////////

#define sysTickHander()  extern volatile TICK  sysTick;  sysTick++
/////////////////////////////////////////////////////////////////////////
void    eXOSSwitch(void); 
HANDLE  CreateTask( void (*thread)( void *arg ), void *arg,int stksize );   	
void    eXOSInit(int mainStk );
void    exitTask(HANDLE t );
void    eXOSDelay(TICK tick );
void    OSStart(void);
TICK    TickGet(void);
void    eXOSYield(void);
void    AddTask( Thread t );
void    TaskSuspend(Thread t);
void    TaskResume(Thread t); 
void    setIdleCallBack(void (*cb)(void));

/////////////////////////////////////////////////////////////////////////

#endif
