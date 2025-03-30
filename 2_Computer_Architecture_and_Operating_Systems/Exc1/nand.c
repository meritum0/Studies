#include "nand.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Struktura reprezentująca jedno z wejść bramki.
typedef struct input
{
    nand_t 		*self;		// Bramka danego wejścia.
    nand_t		*g_signal;	// sygnał - wyjście innej bramki
    bool const		*b_signal;	// sygnał - boolowski
    struct input	*next, *prev;
} input_t;

typedef struct nand 
{
    unsigned int	fan_in;
    bool		visited;
    bool		finished;	// Używana do wykrycia pętli.
    bool		value;		// Wyznaczona wartość na wyjściu.
    ssize_t		path;		// Długość ścieżki krytycznej.
    input_t	       *forward;	// Lista podłączonych wejść.	
    input_t		input[0];
} nand_t;

typedef struct item
{
    nand_t		*gate;
    struct item		*next;
} item_t;

inline ssize_t max( ssize_t a, ssize_t b ) { return a > b ? a : b; }

void reset_and_free_list( item_t *l )
{
    item_t *temp = l;
    while( l )
    {
	temp = l;
	l->gate->visited = l->gate->finished = false;
	l->gate->path = 0;
	l = l->next;
	free( temp );
    }
}

// Odłącza dane wejście od źródła ewentualnie podłączonego sygnału.
void detach_input( input_t *in )
{
    if ( in->b_signal )
    {
	in->b_signal = NULL;
	return;
    }
    if ( in->g_signal )
    {
	if ( in->prev )
	    in->prev->next = in->next;
	else	
	    in->g_signal->forward = in->next;
				
	if ( in->next )
	    in->next->prev = in->prev;
		
	in->g_signal = NULL;
    }
}

int evaluate( nand_t *g, item_t **list )
{
    if ( g->visited )
    {
	if ( g->finished )
	    return g->value;
	else
	{
	    errno = ECANCELED;
	    return -1;	     // Wykryta pętla - to nie jest układ kombinacyjny.
	}
    }

    item_t *new_item = (item_t *)malloc( sizeof(item_t) );
    if ( NULL == new_item )
    {
	errno = ENOMEM;
	return -1;
    }
    new_item->gate = g;
    new_item->next = *list;
    *list = new_item;			// Uaktualnij listę napotkanych bramek.
    g->visited = true;
    g->value = false;

    ssize_t critical_path = 0;
    for ( unsigned int i = 0; i < g->fan_in; i++ )
    {
	if ( !g->input[i].g_signal && !g->input[i].b_signal )
	{
	    errno = ECANCELED;
	    return -1;
	}
	else if ( g->input[i].b_signal )
	{
	    if ( !*(g->input[i].b_signal) )
		g->value = true;
	    critical_path = max( critical_path, 1 );
	}
	else if ( g->input[i].g_signal )
	{
	    int res = evaluate( g->input[i].g_signal, list );
	    if ( res < 0 ) return res;	// Wystąpił gdzieś błąd.
			
	    if ( res == 0 )
		g->value = true;
	    critical_path = max( critical_path, g->input[i].g_signal->path + 1);
	}
    }
    g->path = critical_path;
    g->finished = true;
	
    return g->value ? 1 : 0;
}

//****************************** interfejs nand.h *****************************

// Tworzy nową bramkę o n wejściach.
nand_t* nand_new( unsigned n )
{
    nand_t *nand = (nand_t *)malloc( sizeof(nand_t) + (n * sizeof(input_t)) );
    if ( NULL == nand )
    {
	errno = ENOMEM;
	return NULL;
    }

    nand->fan_in = n;
    nand->visited = nand->finished = nand->value = false;
    nand->path = 0;
    nand->forward = NULL;			 
    for ( unsigned int i = 0; i < n; i++ )
    {
	nand->input[i].self = nand;
	nand->input[i].g_signal = NULL;
	nand->input[i].b_signal = NULL;
	nand->input[i].prev = nand->input[i].next = NULL;
    }
	
    return nand;
}

// Odłącza sygnały wejściowe i wyjściowe danej bramki, a nastepnie usuwa ją.
void nand_delete( nand_t *g )
{
    if ( !g )  return;

    if ( g->forward )
    {
	input_t *temp = g->forward;
	for ( ; temp; g->forward = temp )	// Odłącz wyjście od wszystkich
	{					// podłączonych wejść.
	    temp = g->forward->next;
	    g->forward->g_signal = NULL;
	    g->forward->prev = g->forward->next = NULL;
	}
    }
    
    for ( unsigned int i = 0; i < g->fan_in; i++ ) // Odłącz wszystkie wejścia.
	detach_input( g->input + i );
    free( g );
}

// Podłącza wyjście bramki g_out do wejścia k bramki g_in.
int nand_connect_nand( nand_t *g_out, nand_t *g_in, unsigned k )
{
    if ( !g_out || !g_in || k >= g_in->fan_in )
    {
	errno = EINVAL;
	return -1;
    }

    detach_input( g_in->input + k );
    
    // Dołącz wejście do listy wejść, podłączonych do wyjścia podanej bramki.
    input_t *in = g_in->input + k;
    if ( ! g_out->forward )
	in->next = NULL;
    else
    {
	g_out->forward->prev = in;
	in->next = g_out->forward;
    }
    g_out->forward = in;
    in->prev = NULL;
    in->g_signal = g_out;

    return 0;
}

// Podłącza sygnał boolowski do wejścia k bramki g_in.
int nand_connect_signal( bool const *s, nand_t *g, unsigned k )
{
    if ( !s || !g || k >= g->fan_in )
    {
	errno = EINVAL;
	return -1;
    }
	
    detach_input( g->input + k );
    g->input[k].b_signal = s;
    return 0;
}

// Wyznacza wartość synałów na wyjściach podanych bramek.
ssize_t nand_evaluate( nand_t **g, bool *s, size_t m )
{
    if ( !g || !s || m == 0 )
    {
	errno = EINVAL;
	return -1;
    }
    for ( size_t i = 0; i < m; i++ )
    {
	if ( !g[i] )
	{
	    errno = EINVAL;
	    return -1;
	}
    }

    item_t *list =  NULL;		// Lista napotkanych bramek.	
    ssize_t critical_path = 0;

    for ( size_t i = 0; i < m; i++ )
    {
	int res = evaluate( g[i], &list );
	if ( res < 0 )
	{
	    reset_and_free_list( list );
	    return -1;
	}	
		
	s[i] = res;
	critical_path = max( critical_path, g[i]->path );
    }
    reset_and_free_list( list );
	
    return critical_path;
}

// Wyznacz liczbę wejść bramek podłączonych do wyjścia danej bramki.
ssize_t nand_fan_out( nand_t const *g )
{
    if ( !g )
    {
	errno = EINVAL;
	return -1;
    }
	
    ssize_t cnt = 0;
    for ( input_t *i = g->forward; i != NULL; i = i->next )
	cnt++;
    return cnt;
}

// Zwraca wskaźnik do sygnału boolowskiego lub bramki podłączonej do wejścia k.
void* nand_input( nand_t const *g, unsigned k )
{
    if ( !g || k >= g->fan_in )
    {
	errno = EINVAL;
	return NULL;
    }
    if ( !g->input[k].g_signal && !g->input[k].b_signal )
    {
	errno = 0;
	return NULL;
    }
    else if ( g->input[k].g_signal )
	return g->input[k].g_signal;
    else
	return (void *)(g->input[k].b_signal);
}

// Pozwala iterować po bramkach podłączonych do do wyjścia danej bramki.
nand_t* nand_output( nand_t const *g, ssize_t k )
{
    if ( !g || k >= nand_fan_out(g) )
    {
	errno = EINVAL;
	return NULL;
    }
	
    input_t *ret = g->forward;
    for ( int i = 0; i < k; i++ )
	ret = ret->next;
    return ret->self;
}
