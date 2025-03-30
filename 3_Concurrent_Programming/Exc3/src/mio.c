#include "mio.h"
#include "err.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>

#include "debug.h"
#include "executor.h"
#include "waker.h"

// Maximum number of events to handle per epoll_wait call.
#define MAX_EVENTS 64

struct Mio {
    // TODO: add required fields
    int	epoll_fd;
    struct epoll_event events[ MAX_EVENTS ];
    Executor	*executor;
};

// TODO: delete this once not needed.
#define UNIMPLEMENTED (exit(42))

Mio* mio_create(Executor* executor)
{
    Mio	*mio = malloc( sizeof(struct Mio) );

    if ( mio == NULL )
	syserr( "[mio_create] malloc() failed" );

    mio->executor = executor;

    mio->epoll_fd = epoll_create1( 0 );
    if ( mio->epoll_fd == -1 )
	syserr( "[mio_create] epoll_create1() failed" );

    return mio;
}

void mio_destroy(Mio* mio)
{
    close( mio->epoll_fd );
    free( mio );
}

int mio_register(Mio* mio, int fd, uint32_t events, Waker waker)
{
    debug("Registering (in Mio = %p) fd = %d with\n", mio, fd);
  
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = NULL;

    int rc = epoll_ctl( mio->epoll_fd, EPOLL_CTL_ADD, fd, &ev );

    if ( rc == -1   &&  errno != EEXIST )
 	syserr( "[mio_register] epoll_ctl(add) failed" );

    ev.data.ptr = malloc( sizeof(Waker) );
    *(Waker *)(ev.data.ptr) = waker;
    
    if ( epoll_ctl( mio->epoll_fd, EPOLL_CTL_MOD, fd, &ev ) != 0 )
	syserr( "[mio_register] epoll_ctl(mod) failed" );
    
    return 0;
}

int mio_unregister(Mio* mio, int fd)
{
    debug("Unregistering (from Mio = %p) fd = %d\n", mio, fd);

    return epoll_ctl( mio->epoll_fd, EPOLL_CTL_DEL, fd, NULL );
}

void mio_poll(Mio* mio)
{
    debug("Mio (%p) polling\n", mio);

    int rc;
    do
    {
	rc = epoll_wait( mio->epoll_fd, mio->events, MAX_EVENTS, -1 );
    } while (rc == -1  && errno == EINTR );

    if ( rc == -1 )
	syserr( "[mio_poll] epoll_wait() failed" );
	
    for( int i = 0; i < rc; i++ )
    {
	if ( ( mio->events[i].events) == 0 )
	    continue;

	waker_wake( mio->events[i].data.ptr );
	free( mio->events[i].data.ptr ); 
    }
}
