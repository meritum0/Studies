#include "common/io.h"
#include "common/sumset.h"
#include "common/err.h"
#include <stddef.h>
#include <stdatomic.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct tasks tasks;

typedef struct elem
{
    Sumset  a;
    Sumset  b;
    struct elem* prev;
    struct elem* next;
    atomic_int ptr_count;
} elem;

typedef struct
{
    elem* head;
    int changes;
    int	pad;
} Stack;

typedef struct tasks {
    int thread_count;
    atomic_int waiting;
    _Atomic Stack stack;
    pthread_mutex_t mutex;
    pthread_cond_t wait_for_task;
} tasks;

static InputData input_data;
static Solution best_solution;
static tasks tasks_to_do = {0};

static pthread_mutex_t solution_mutex = PTHREAD_MUTEX_INITIALIZER;

void tasks_push( tasks* t, elem* e )
{
    elem* new_head = e;

    Stack next  = {0};
    Stack prev = atomic_load( &t->stack );
    do
    {
	new_head->next = prev.head;
	next.head = new_head;
	next.changes = prev.changes + 1;
    } while( !atomic_compare_exchange_weak( &t->stack, &prev, next ) );

    if ( t->waiting )
	ASSERT_SYS_OK( pthread_cond_signal( &t->wait_for_task ));
}

elem* tasks_pop(tasks* t)
{
    t->waiting++;
    Stack next = {0};
    Stack prev = atomic_load( &t->stack );
    do
    {
	if (  prev.head == NULL )
	{
	    ASSERT_SYS_OK( pthread_mutex_lock( &t->mutex ) );
	    
	    while( prev.head == NULL )
	    {
		if ( t->thread_count == t->waiting )
		{
		    ASSERT_SYS_OK( pthread_cond_signal( &t->wait_for_task ));
		    ASSERT_SYS_OK( pthread_mutex_unlock( &t->mutex));
		    return NULL;
		}
		ASSERT_SYS_OK( pthread_cond_wait( &t->wait_for_task, &t->mutex ) );
		prev = atomic_load( &t->stack );
	    }
	    ASSERT_SYS_OK( pthread_mutex_unlock( &t->mutex));
	}
    
	next.head = prev.head->next;
	next.changes = prev.changes + 1;

    } while( !atomic_compare_exchange_weak( &t->stack, &prev, next) );

    t->waiting--;
    
    return prev.head;
}

elem* new_elem( Sumset *a, Sumset *b, elem* prev )
{
    elem* e = (elem*)malloc(sizeof(elem));
    *e = (elem){ *a, *b, prev, NULL, 0 };
    return e;
}

void swap_sets(elem* in)
{
    Sumset s = in->a;
    in->a= in->b;
    in->b = s;
}

void solve( elem* in )
{
    if ( in->a.sum > in->b.sum )
	swap_sets(in);

    if ( is_sumset_intersection_trivial(&in->a, &in->b) )  // s(a) ∩ s(b) = {0}.
    {    
	Sumset  a_with_i;
	for ( size_t i = in->a.last; i <= input_data.d; ++i )
	    if ( ! does_sumset_contain( &in->b, i) )
		in->ptr_count++; // atomic ++

	int tmp = in->ptr_count;
	bool children = false;
	for ( size_t i = in->a.last; i <= input_data.d; ++i )
	{
	    if ( ! does_sumset_contain( &in->b, i) )
	    {
		children = true;
		tmp--;
		sumset_add( &a_with_i, &in->a, i );
		elem* new_in = new_elem( &a_with_i, &in->b, in );
		
		if ( /*tasks_to_do.waiting  &&*/  i == in->a.last && i < input_data.d*2/3 )
		{
		    tasks_push( &tasks_to_do, new_in );
		}
		else
		    solve( new_in );
	    }
	    if ( tmp == 0 ) break;
	}
	if ( ! children )
	    free(in);
    }
    else
    {
	if ( in->a.sum == in->b.sum  &&  get_sumset_intersection_size( &in->a, &in->b ) == 2 )
	{
	    if ( in->b.sum > best_solution.sum )
	    {
		ASSERT_SYS_OK( pthread_mutex_lock( &solution_mutex ));
		solution_build( &best_solution, &input_data, &in->a, &in->b );
		ASSERT_SYS_OK( pthread_mutex_unlock( &solution_mutex ));
	    }   
	}
	
	while ( in->prev  &&  in->prev->ptr_count == 1 )
	{
	    elem* temp = in->prev;

	    in->prev = in->prev->prev;

	    free( temp);
	}

	if (in->prev)
	    in->prev->ptr_count--;
	
	free(in); 
    }
    return;
}

void* worker( void* arg )
{
    elem* new_in = NULL;

    while ( (new_in = tasks_pop( &tasks_to_do )) != NULL )
    {
        solve(new_in);

    }
    return NULL;
}

int main()
{
    input_data_read(&input_data);
    solution_init(&best_solution);

    tasks_to_do.thread_count = input_data.t;
    tasks_to_do.waiting = 0;
    tasks_to_do.stack = (Stack){0};
    ASSERT_SYS_OK(pthread_mutex_init(&tasks_to_do.mutex, NULL));
    ASSERT_SYS_OK(pthread_cond_init(&tasks_to_do.wait_for_task, NULL));

    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * input_data.t);
    if (input_data.a_start.sum > input_data.b_start.sum)  // swap
    {
        Multiset temp1 = input_data.a_in;
        input_data.a_in = input_data.b_in;
        input_data.b_in = temp1;

        Sumset temp2 = input_data.a_start;
        input_data.a_start = input_data.b_start;
        input_data.b_start = temp2;
    }

    elem* in = new_elem( &input_data.a_start, &input_data.b_start, NULL );
    tasks_push( &tasks_to_do, in );
     
    for (int i = 0; i < input_data.t; i++)
    {
	ASSERT_SYS_OK( pthread_create( &threads[i], NULL, worker, NULL ) );
    }

    for (int i = 0; i < input_data.t; i++)
    {
        ASSERT_SYS_OK( pthread_join(threads[i], NULL ));
    }

    free(threads);

    solution_print(&best_solution);

    ASSERT_SYS_OK( pthread_mutex_destroy( &tasks_to_do.mutex ));
    ASSERT_SYS_OK( pthread_cond_destroy( &tasks_to_do.wait_for_task ));
 
    return 0;
}



