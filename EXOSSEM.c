/*************************************************************************\
 * File Name	: EXOSSEM.c			       		      	 *
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

#include "lists.h"
#include "EXOSSEM.h"

//////////////////////////////////////////////////////////////////////////

extern Thread current; 
extern Thread ready;
/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosSemInit                                       *
 *   Returns 	     : void                                              *
 *   Parameters      : XOSSem   int                                      *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXosSemInit(XOSSem *s, int count) 
{
	assert(s);
	
	s->count = count;
	s->queue = NULL;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosSemWait                                       *
 *   Returns 	     : void                                              *
 *   Parameters      : XOSSem                                            *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXosSemWait(XOSSem *s) 
{
	assert(s);
	
	if (s->count <= 0) 
	{
		put(current, (Thread *)&s->queue);
		eXOSSwitch();
	} else
		--s->count;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : eXosSemSignal                                     *
 *   Returns 	     : void                                              *
 *   Parameters      : XOSSem                                            *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void eXosSemSignal(XOSSem *s) 
{
	assert(s);
	
	if (s->count == 0 && !isempty(s->queue)) 
	{
		Thread t = get((Thread *)&s->queue);
		put(t, &ready);
	} 
	else
		++s->count;
}

////////////////////////////////////////////////////////////////////////
