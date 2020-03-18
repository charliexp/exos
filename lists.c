/*************************************************************************\
 * File Name	: lists.c			         	         *
 * --------------------------------------------------------------------- *
 * Title	: EXOS --- Event-Driven system                           *
 * Revision	: V1.0                                                   *
 * Notes	:                                                        *
 * --------------------------------------------------------------------- *
 * Revision History:                                                     *
 *   When	      Who	  Revision	 Description of change   *
 * -----------	  -----------	 ---------      ------------------------ *
 * 20-09-2010	  charlie_weng	   V1.0	         Created the program     *
 *                                                                       * 
\*************************************************************************/


#include "lists.h"

/*************************************************************************\
 *                                                                       *
 *   Function name   : put                                               *
 *   Returns 	     : void                                              *
 *   Parameters      : Thread Thread*                                    *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void put(Thread t, Thread *q) 
{
	assert(t);
	assert(t->inqueue == NULL && t->link == NULL);
	
	if (*q) 
	{
		t->link = (*q)->link;
		(*q)->link = t;
	} 
	else
		t->link = t;
	*q = t;
	t->inqueue = q;
}
/*************************************************************************\
 *                                                                       *
 *   Function name   : get                                               *
 *   Returns 	     : Thread                                            *
 *   Parameters      : Thread*                                           *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
Thread get(Thread *q) 
{
	Thread t;
	
	if(isempty(*q))
	     	return NULL;
	      
	t = (*q)->link;
	if (t == *q)
		*q = NULL;
	else
		(*q)->link = t->link;
		
	if(t->inqueue != q)
	         return NULL;
	       
	t->link    = NULL;
	t->inqueue = NULL;
	
	return t;
}

/*************************************************************************\
 *                                                                       *
 *   Function name   : delete                                            *
 *   Returns 	     : void                                              *
 *   Parameters      : Thread Thread*                                    *
 *   Purpose         :                                                   *
 *                                                                       *
\*************************************************************************/
void delete(Thread t, Thread *q) 
{
	Thread p;

	assert(t->link && t->inqueue == q);
	assert(!isempty(*q));

	for (p = *q; p->link != t; p = p->link)
		;
	if (p == t)
		*q = NULL;
	else 
	{
		p->link = t->link;
		if (*q == t)
			*q = p;
	}
	t->link    = NULL;
	t->inqueue = NULL;
}

///////////////////////////////////////////////////////////////////////////
