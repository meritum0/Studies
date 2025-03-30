#include "executor.h"
#include "err.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "future.h"
#include "mio.h"
#include "waker.h"

/**
 * @brief Structure to represent the current-thread executor.
 */
struct Executor {
    Mio		*mio;
    size_t	 front;
    size_t	 rear;
    size_t	 queue_size;
    size_t	 max_queue_size;
    int		 pending;
    Future      *queue[0];
};

// TODO: delete this once not needed.
#define UNIMPLEMENTED (exit(42))

Executor* executor_create(size_t max_queue_size)
{
    Executor *executor = calloc( 1, sizeof(struct Executor) + max_queue_size * sizeof(Future  *));

    if ( executor == NULL )
	syserr( "[executor_create] malloc() failed" );

    executor->mio = mio_create( executor );
    executor->front = executor->rear = executor->queue_size = executor->pending = 0;
    executor->max_queue_size = max_queue_size;
    
    return executor;
}

void waker_wake(Waker* waker)
{
    executor_spawn( waker->executor, waker->future );
    Executor *e = waker->executor;
    e->pending--;
}

void executor_spawn(Executor* executor, Future* fut)
{
    if ( executor->queue_size == executor->max_queue_size )
	fatal( "executor_spawn() full" );
 
    executor->queue[ executor->rear ] = fut;
    executor->queue[ executor->rear ]->is_active = true;
    executor->rear = (executor->rear + 1) % executor->max_queue_size;
    executor->queue_size++;
}

void executor_run(Executor* executor)
{
    while( executor->queue_size  ||  executor->pending > 0 )
    {
	while ( executor->queue_size > 0 )
	{
	    Future *f = executor->queue[ executor->front ];

	    if ( f->is_active )
	    {
		if ( FUTURE_PENDING == f->progress( f, executor->mio, (Waker){ executor, f }) )
		    executor->pending++;
		else
		    f->is_active = false;
	    }
	    
	    executor->front = (executor->front + 1) % executor->max_queue_size;
	    executor->queue_size--;
	}

	if ( executor->pending > 0 )
	    mio_poll( executor->mio );
    }
}

void executor_destroy(Executor* executor)
{
    if ( executor )
    {
	if ( executor->mio )
	    mio_destroy( executor->mio );
	
	free( executor );
    }
}
