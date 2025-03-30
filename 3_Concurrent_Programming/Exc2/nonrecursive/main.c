#include "common/io.h"
#include "common/sumset.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct elem
{
    Sumset  	 a;
    Sumset  	 b;
    struct elem* prev;
    struct elem* next;
    bool 	 is_last;
} elem;

void push( elem* s, Sumset *a, Sumset *b, elem* prev, bool is_last )
{
    elem* e = (elem*)malloc(sizeof(elem));
    *e = (elem){ *a, *b, prev, s->next, is_last };
    s->next = e;
}

elem* pop( elem* s )
{
    if ( s->next == NULL )  return NULL;

    elem* temp = s->next;
    s->next = temp->next;
    return temp;
}

void swap_sets( elem* in )
{
    Sumset s = in->a;
    in->a = in->b;
    in->b = s;
}

int main()
{
    InputData input_data;
    input_data_read(&input_data);

    Solution best_solution;
    solution_init( &best_solution );

    elem stos = {0};
    push( &stos, &input_data.a_start, &input_data.b_start, NULL, true );

    Sumset  a_with_i;
    while( stos.next != NULL )		// not empty?
    {
        elem* in = pop( &stos );
        if ( in->a.sum > in->b.sum)
            swap_sets( in );

        if ( is_sumset_intersection_trivial( &in->a, &in->b ) )
	{
            bool     is_last = true;
	    for (size_t i = in->a.last; i <= input_data.d; ++i)
	    {
                if ( !does_sumset_contain( &in->b, i ) )
		{
                    sumset_add( &a_with_i, &in->a, i );
                    push( &stos, &a_with_i, &in->b, in, is_last );
		    is_last = false;
                }
            }
	    if ( is_last )
		free(in);
        }
        else
	{
            if ( in->a.sum == in->b.sum  &&  get_sumset_intersection_size( &in->a, &in->b ) == 2 )
		if ( in->b.sum > best_solution.sum )
		    solution_build( &best_solution, &input_data, &in->a, &in->b );

	    while( in->is_last  &&  in->prev )
	    {
		elem* temp = in->prev;
		in->is_last = in->prev->is_last; 
		in->prev = in->prev->prev;
		free(temp);
	    }
	    free(in);
        }
    }
    solution_print( &best_solution );
    return 0;
}

