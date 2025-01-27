/*====================================================================*
   -  Copyright (C) 2001 Leptonica.  All rights reserved.
   -
   -  Redistribution and use in source and binary forms, with or without
   -  modification, are permitted provided that the following conditions
   -  are met:
   -  1. Redistributions of source code must retain the above copyright
   -     notice, this list of conditions and the following disclaimer.
   -  2. Redistributions in binary form must reproduce the above
   -     copyright notice, this list of conditions and the following
   -     disclaimer in the documentation and/or other materials
   -     provided with the distribution.
*====================================================================*/

#ifndef  LEPTONICA_STACK_H
#define  LEPTONICA_STACK_H

/*!
 * \file stack.h
 *
 * <pre>
 *       Expandable pointer stack for arbitrary void* data.
 *
 *       The L_Stack is an array of void * ptrs, onto which arbitrary
 *       objects can be stored.  At any time, the number of
 *       stored objects is stack->n.  The object at the bottom
 *       of the stack is at array[0]; the object at the top of
 *       the stack is at array[n-1].  New objects are added
 *       to the top of the stack, at the first available location,
 *       which is array[n].  Objects are removed from the top of the
 *       stack.  When an attempt is made to remove an object from an
 *       empty stack, the result is null.   When the stack becomes
 *       filled, so that n = nalloc, the size is doubled.
 *
 *       The auxiliary stack can be used to store and remove
 *       objects for re-use.  It must be created by a separate
 *       call to pstackCreate().  [Just imagine the chaos if
 *       pstackCreate() created the auxiliary stack!]
 *       pstackDestroy() checks for the auxiliary stack and removes it.
 * </pre>
 */

/*! Expandable pointer stack for arbitrary void* data.
 * Note that array[n] is the first null ptr in the array
 */
struct L_Stack {
	l_int32 nalloc;      /*!< size of ptr array              */
	l_int32 n;               /*!< number of stored elements      */
	void           ** array; /*!< ptr array                      */
	struct L_Stack  * auxstack; /*!< auxiliary stack                */
};

typedef struct L_Stack L_STACK;

#endif /*  LEPTONICA_STACK_H */
