#include "future_combinators.h"
#include <stdlib.h>

#include "future.h"
#include "waker.h"

// TODO: delete this once not needed.
#define UNIMPLEMENTED (exit(42))

static FutureState future_then_progress(Future* base, Mio* mio, Waker waker)
{
    ThenFuture	*self = (ThenFuture*)base;
    FutureState	 state = FUTURE_FAILURE;
    
    if ( ! self->fut1_completed )
    {
	state = self->fut1->progress( self->fut1, mio, waker );
	if ( state == FUTURE_COMPLETED )
	{
	    self->fut1_completed = true;
	    self->fut2->arg = self->fut1->ok;
	}
	else if ( state == FUTURE_FAILURE )
	{
	    self->fut1_completed = true;
	    self->base.errcode = THEN_FUTURE_ERR_FUT1_FAILED;
	}
    }

    if ( self->fut1_completed  &&  self->fut1->errcode == FUTURE_SUCCESS )
    {
	state = self->fut2->progress( self->fut2, mio, waker );
	if (  state == FUTURE_COMPLETED )
	    self->base.ok = self->fut2->ok;
	else if ( state == FUTURE_FAILURE )
	    self->base.errcode = THEN_FUTURE_ERR_FUT2_FAILED;
    }
	
    return state;
}

ThenFuture future_then(Future* fut1, Future* fut2)
{
    return (ThenFuture) {
	.base = future_create( future_then_progress),
	.fut1 = fut1,
	.fut2 = fut2,
	.fut1_completed = false
    };
}

static FutureState future_join_progress(Future* base, Mio* mio, Waker waker)
{
    JoinFuture   *self = (JoinFuture*)base;

    if ( ! self->fut1_completed )
	self->fut1_completed = (FUTURE_PENDING != self->fut1->progress( self->fut1, mio, waker ) );

    if ( ! self->fut2_completed )
	self->fut2_completed = (FUTURE_PENDING != self->fut2->progress( self->fut2, mio, waker ) );

    if ( self->fut1_completed  &&  self->fut2_completed )
    {
	self->result.fut1.errcode = self->fut1->errcode;
	self->result.fut1.ok      = self->fut1->ok;
	self->result.fut2.errcode = self->fut2->errcode;
	self->result.fut2.ok      = self->fut2->ok;
	
	if ( self->fut1->errcode == FUTURE_SUCCESS  &&  self->fut2->errcode == FUTURE_SUCCESS )
	    return FUTURE_COMPLETED;
	else
	{
	    if ( self->fut1->errcode != FUTURE_SUCCESS  &&  self->fut2->errcode != FUTURE_SUCCESS )
		self->base.errcode = JOIN_FUTURE_ERR_BOTH_FUTS_FAILED;
	    else if ( self->fut1->errcode != FUTURE_SUCCESS )
		self->base.errcode = JOIN_FUTURE_ERR_FUT1_FAILED;
	    else
		self->base.errcode = JOIN_FUTURE_ERR_FUT2_FAILED;

	    return FUTURE_FAILURE;
	}
    }
    else
        return FUTURE_PENDING;
}

JoinFuture future_join(Future* fut1, Future* fut2)
{
    return (JoinFuture) {
	.base = future_create( future_join_progress),
	.fut1 = fut1,
	.fut2 = fut2,
	.fut1_completed = false,
	.fut2_completed = false,
	.result = {}
    };
}

static FutureState future_select_progress(Future* base, Mio* mio, Waker waker)
{
    SelectFuture  *self = (SelectFuture*)base;
    FutureState	   state1 = FUTURE_FAILURE;
    FutureState	   state2 = FUTURE_FAILURE;

    if ( self->which_completed == SELECT_FAILED_BOTH )
    {
	self->base.errcode = SELECT_FAILED_BOTH;
	return FUTURE_FAILURE;
    }


    if ( self->fut1->errcode == FUTURE_SUCCESS  && self->which_completed != SELECT_COMPLETED_FUT1 )
    {
	state1 = self->fut1->progress( self->fut1, mio, waker );

	if ( state1 == FUTURE_COMPLETED )
	{
	    if ( self->which_completed == SELECT_COMPLETED_FUT2 )
		return FUTURE_COMPLETED;

	    self->which_completed = SELECT_COMPLETED_FUT1;

	    if ( self->which_completed == SELECT_FAILED_FUT2 )
		 return FUTURE_FAILURE;
	}
	else if ( state1 == FUTURE_FAILURE )
	{

	}
    }

    if ( self->fut2->errcode == FUTURE_SUCCESS  && self->which_completed != SELECT_COMPLETED_FUT2 )
    {
	state2 = self->fut2->progress( self->fut2, mio, waker );
	if ( state2 == FUTURE_COMPLETED  )
	{
	    if ( self->which_completed == SELECT_COMPLETED_FUT1 )
		return FUTURE_COMPLETED;

	    self->which_completed = SELECT_COMPLETED_FUT2;
	    if ( self->which_completed == SELECT_FAILED_FUT1 ) 
		 return FUTURE_FAILURE;
	}
	else if ( state1 == FUTURE_FAILURE )
	{

	}
	
    }

    return FUTURE_PENDING;
}

SelectFuture future_select(Future* fut1, Future* fut2)
{
    return (SelectFuture) {
	.base = future_create(future_select_progress),
	.fut1 = fut1,
	.fut2 = fut2,
	.which_completed = SELECT_COMPLETED_NONE
    };
}
