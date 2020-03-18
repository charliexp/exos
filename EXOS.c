/*************************************************************************\
 * File Name	: EXOS.c			      		         *
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

#include "EXOS.h"
#include "lists.h"

//////////////////////////////////////////////////////////////////////////
#define  SYS_THREAD_NUM                  1
#define  IDLE_STK_SIZE                  16
//////////////////////////////////////////////////////////////////////////
static   STK     stktop;
static   STK     oldsp;

         Thread  ready = NULL;
static   Thread  delay = NULL;
static   Thread  block = NULL;
         
         Thread  current; 
         Thread  idle;

static   struct  Thread ThreadTcb[MAX_THREAD_NUM+SYS_THREAD_NUM];
static   BYTE    nthreads = 0;
volatile TICK    sysTick;

//////////////////////////////////////////////////////////////////////////
static   void    (*sysCallBack)( void )= NULL;
static   void    timerTick(void *arg);    
static   void    InsertDelayList( void );  
static   void    timerTickCheck( void );

/*************************************************************************\
 *                                                                       *
 *   Function name   : setIdleCallBack                                   *
 *   Returns 	     : void                                              *
 *   Parameters      : void (*cb)(void)                                  *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void setIdleCallBack(void (*cb)(void))
{
	sysCallBack = cb;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : XOSInit                                           *
 *   Returns 	     : void                                              *
 *   Parameters      : STK                                               *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXOSInit( int mainStk )
{
	GET_STACK(&stktop);
	
	stktop = stktop - IDLE_STK_SIZE;
		 	
        idle = CreateTask(timerTick, NULL, mainStk);  	 	 
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : CreateTask                                        *
 *   Returns 	     : HANDLE                                            *
 *   Parameters      :                                                   *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
HANDLE  CreateTask( void (*thread)( void *arg ), 
          void *arg, int stkSize )                      
{	 
	 if(thread == NULL)
	       return NULL;
	
	 current = &ThreadTcb[nthreads];	
	 GET_STACK(&oldsp);
	 SET_STACK(&stktop);
	 
	 if ( !MTCX_SAVE(current) ) 
	 {
	 	SET_STACK(&oldsp);
	 	
  	 	current->thread = thread;
  	 	current->arg    = arg;
 		
         	put(current, &ready);
         	
         	stktop -= stkSize;
         	nthreads++;
         }     	    
         else
	 { 
	 	while(1)
	 	{
	 		current->thread( current->arg );
	 		exitTask(current);
		}
	 }
	 return current;
}  		

/*************************************************************************\
 *                                                                       *
 *   Function name   : exitTask                                          *
 *   Returns 	     : void                                              *
 *   Parameters      : Thread                                            *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void exitTask(HANDLE t )
{
	if( t!= current )
		delete(t,t->inqueue);
	else
		eXOSSwitch();
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : AddTask                                           *
 *   Returns 	     : void                                              *
 *   Parameters      : Thread                                            *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void  AddTask( Thread t )
{
	put( t, &ready );
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : TaskSuspend                                       *
 *   Returns 	     : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void TaskSuspend(Thread t) 
{
	delete( t, t->inqueue);
	put(t, &block);
	
	if( t == current )
	{
	       eXOSSwitch();
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : TaskResume                                        *
 *   Returns 	     : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void TaskResume(Thread t) 
{ 
	if( t->inqueue == &block )
	{
		delete( t, &block);
		put(t,&ready);
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : OSStart                                           *
 *   Returns 	     : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void OSStart(void) 
{
	ISR_EN();

	current = get(&ready);
	
	if(!current)
		current = idle;
			
	MTCX_RESTORE(current);
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : TickGet                                           *
 *   Returns 	     : TICK                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
TICK TickGet( void )
{
	TICK tick;
	
	ENTER_CRITICAL(); 
	tick  = sysTick;
	EXIT_CRITICAL(); 
	
 	return tick;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : eXOSSwitch                                        *
 *   Returns 	     : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXOSSwitch(void) 
{
	Thread t = current;
	
	current = get(&ready);	
	if(!current||(t == current))
		current = idle;	
			
	_swtch(t, current);
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : eXOSYield                                         *
 *   Returns 	     : void                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXOSYield(void) 
{
	put(current, &ready);
	eXOSSwitch();
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : eXOSDelay                                         *
 *   Returns 	     : TICK                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXOSDelay(TICK tick )
{
	if(tick)
	{
		current->tick  = TickGet()+ tick; 
		InsertDelayList();
		eXOSSwitch();
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : InsertDelayList                                   *
 *   Returns 	     : TICK                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void InsertDelayList( void )
{
	Thread t;
	Thread q;
	
	if( delay == NULL )
	{
		put( current, &delay );
	}
	else
	{
                q = delay;
		t = delay->link;
		
		while(1)
		{
			if( (t->tick - current->tick) < TICK_MAX/2 )
			{
				current->link = q->link;
				q->link = current;
				current->inqueue = &delay;
				break;
			}
			else
			{
				if( t == delay )
				{
					put(current, &delay);
					break;
				}
				q = t;
				t = t->link;
			}
		}
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : timerTickCheck                                    *
 *   Returns 	     : TICK                                              *
 *   Parameters      : void                                              *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void timerTickCheck( void )
{
	Thread  t;
	TICK    currTick;
	
	if( delay )
	{
		currTick = TickGet();
		
		while(!isempty(delay))
		{
			t = delay->link;
			
			if((currTick - t->tick) < TICK_MAX/2)
			{
				t = get(&delay);
				put(t,&ready);
				currTick = TickGet();
			}
			else
			{
			        break;
			}
		}
	}
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : timerTick                                         *
 *   Returns 	     : void                                              *
 *   Parameters      : void*                                             *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
static void timerTick(void *arg)
{
	while(1)
	{   		
		timerTickCheck();
				
		eXOSYield();
		
		if( sysCallBack )
    		{
    			sysCallBack();
    		}
	}
}

//////////////////////////////////////////////////////////////////////////

