/* Autor kodu: Aleksander Lukosek
   Review: Jakub Wielocha
   Jedynymi mankamentami sa uzycia niestandardowej skladni (takiej jak w lini 15 czy 86), 
   ktore moga sprawiac, ze nie wiadomo co sie dokladnie w kodzie dzieje. W ogolnosci,
   postawilbym na prostszy zapis kodu i przez to jego wydluzenie, zamiast uzywac specyficznej
   skladni i go mocno skracac.
   Poza uzyciami niestandardowej skladni kod jest bardzo czytelny i przejrzysty.
	
   Odpowiedź na review:
   Jeśli chodzi o wspomnianą składnię, to użyłem operatora warunkowego Ternery z dwoma lvalue tego
   samego typu oraz funkcji Lambda, żeby nie powielać dwa razy kodu.
*/
#include <iostream>
#include "kol.h"

struct lista
{
	interesant *head, *tail;
};

std::vector<lista> okienka;

bool empty_queue( lista q ) { return q.head->l2 == q.tail; }

interesant *usun( interesant *popped )		// usun pierwszy w kolejce
{
	(popped->l1->l2 == popped  ?  popped->l1->l2 : popped->l1->l1) = popped->l2;
	(popped->l2->l1 == popped  ?  popped->l2->l1 : popped->l2->l2) = popped->l1;
	return popped;
}

void dodaj( interesant *i, int k )		// dodaj na koncu koleki
{
	i->l1 = okienka[k].tail->l1;
	i->l2 = okienka[k].tail;
	okienka[k].tail->l1 = i;
	(i->l1->l2 == i->l2  ? i->l1->l2 : i->l1->l1) = i;
}

/****************************** interface kol.h ************************************/

int numerek(interesant *i){ return i->nr; }

void otwarcie_urzedu( int m )
{
	okienka.resize(m);
	for( lista &o : okienka )
	{
		o.head = (interesant*)malloc( sizeof(interesant) );
		o.tail = (interesant*)malloc( sizeof(interesant) );
		o.head->l1 = o.tail->l2 = NULL;
		o.head->l2 = o.tail;
		o.tail->l1 = o.head;
	}
}

interesant *nowy_interesant( int k )
{
	static int nowy_nr = 0;
	interesant *res = (interesant*)malloc( sizeof(interesant) );
	res->nr = nowy_nr++;
	dodaj( res, k );
	return res;
}

interesant *obsluz( int k )
{
	if( empty_queue( okienka[k] ) ) return NULL;
	return usun( okienka[k].head->l2 );
}

void zmiana_okienka( interesant *i, int k )
{
	dodaj( usun( i ), k );
}

void zamkniecie_okienka( int k1, int k2 )
{
	if( empty_queue( okienka[k1] ) ) return;
	interesant *first = okienka[k1].head->l2;
	interesant *last = okienka[k1].tail->l1;
	
	// dodanie kolejki z k1 na koniec k2
	(okienka[k2].tail->l1->l2 == okienka[k2].tail ?
		okienka[k2].tail->l1->l2 : okienka[k2].tail->l1->l1) = first;
	(first->l1 == okienka[k1].head ? first->l1 : first->l2) = okienka[k2].tail->l1;
	( last->l2 == okienka[k1].tail ?  last->l2 : last->l1)  = okienka[k2].tail;
	okienka[k2].tail->l1 = last;
	
	okienka[k1].head->l2 = okienka[k1].tail;	// wyzeruj kolejke
	okienka[k1].tail->l1 = okienka[k1].head;	// do okienka k1
}

std::vector<interesant*> fast_track( interesant *i1, interesant *i2 )
{
	if( i1 == i2 )  return { usun( i1 ) };
	
	auto check_dir = [&]( interesant *dir, interesant **after_i2, interesant *prev )
	{
		while( dir->l1  &&  dir->l2 )	// z której strony jest i2 ?
		{
			if( dir == i2 )  *after_i2 = (i2->l2 == prev ? i2->l1 : i2->l2);
			dir = ( dir->l2 == prev ? (prev=dir)->l1 : (prev=dir)->l2 );
		}
	};
	
	interesant *after_i1 = i1->l2, *after_i2 = NULL;
	check_dir( i1->l1, &after_i2, i1 );
	if( !after_i2 )		// nie znalezliśmy i2 po stronie l1
	{
		after_i1 = i1->l1;
		check_dir( i1->l2, &after_i2, i1 );
	}
	
	// usuniecie grupy interesantow z kolejki
	( after_i2->l2 == i2 ? after_i2->l2 : after_i2->l1 ) = after_i1;
	( after_i1->l1 == i1 ? after_i1->l1 : after_i1->l2 ) = after_i2;
	
	std::vector<interesant*> res{ i1 };
	interesant *cur  = i1,  *prev = after_i1;
	while ( cur != i2 ) 
		res.push_back( cur = (cur->l2 == prev ? (prev=cur)->l1 : (prev=cur)->l2) );
	return res;
}

void naczelnik( int k )
{
	if( empty_queue( okienka[k]) ) return;
	std::swap( okienka[k].tail->l1, okienka[k].head->l2 );
	(okienka[k].tail->l1->l1 == okienka[k].head ?
		okienka[k].tail->l1->l1 : okienka[k].tail->l1->l2) = okienka[k].tail;
	(okienka[k].head->l2->l2 == okienka[k].tail ?
		okienka[k].head->l2->l2 : okienka[k].head->l2->l1) = okienka[k].head;
}

std::vector<interesant*> zamkniecie_urzedu()
{
	std::vector<interesant*> res;
	for( auto &o : okienka )
		{
			interesant *cur = o.head->l2, *prev = o.head;
			for(; cur != o.tail;  cur = (cur->l1 == prev ? (prev=cur)->l2 : (prev=cur)->l1 ) )
				res.push_back( cur );
			free( o.head );
			free( o.tail );
		}
	return res;
}

