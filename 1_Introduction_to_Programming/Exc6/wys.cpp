/* Autor kodu: Aleksander Lukosek
   peer review: Iga Jeznacka
   komentarz reviewera:
   warto dodac komentarz co trzymamy w dp
   w update moim zdaniem warto by rozbic fora na 2 przypadki, osobno dla ans i !ans, bo teraz slabo widac, co sie dzieje w tej petli
   mozna rozwazyc przekazywanie state jako zmiennej lokalnej, mysle ze np. w key to zwiekszylo by czytelnosc
   poza tym wszystko w porzadku
	
   Odpowiedz: Zastosowalem sie do pierwszej uwagi.
*/
#include "wys.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#define BIG 100;
using namespace std;

int n, k, g, not_pos;	// not_pos liczy, ile wartosci jest juz zdyskwalifikowanych
vector<int> state(13,0);	 // state[i] mowi, ile razy Ala musialaby sklamac, zeby i bylo szukana liczba
unordered_map<long long, int> dp; // dp[i] mowi, w ilu pytaniach skonczymy gre w stanie i

long long key()	// (prawie)zamiana na system (k+2)-kowy
{
	long long res = 0;
	for( int i = 1; i <= n; i++ )
		res += (state[(unsigned)i] >= k + 1 ? k + 1 : state[(unsigned)i] ) * (int)pow( k+2, i-1);
	return res;
}

void update( unsigned q, bool ans, int v=1 )	 // aktualizuje(v=1) / cofa(v=-1) zmiany dla pytania q
{
	for( unsigned i = ( ans ? q : q - 1 ); i >= 1 && (int)i <= n; ( ans ? i++ : i-- ) )
	{
		if( state[i] == k + ( v == 1 ? 0 : 1 ) ) not_pos += v;
		state[i] += v;
	}
}

int eval( long long cur )	// wypelnia dp[]
{
	if( not_pos == n - 1 ) return 0;	// mamy odpowiedz
	if( dp[cur] ) return dp[cur];
	
	dp[cur] = BIG;
	for( unsigned i = 2; (int)i <= n; i++ )	// zadajemy kazde sensowne pytanie
	{
		update( i, false );
		int temp = eval( key() );
		update( i, false, - 1 );
		if( temp + 1 < dp[cur] )
		{
			update( i, true );
			temp = max( temp, eval( key() ) );
			update( i, true, -1 );
			
			dp[cur] = min( dp[cur], temp + 1 );
		}
	}
	return dp[cur];
}

void solve()	// funkcja grajaca
{
	while( not_pos != n - 1 )
	{
		unsigned question = 0;
		long long cur = key();
		for( unsigned i = 2; (int)i <= n; i++ )	// szukamy najlepszego pytania
		{
			update( i , true );
			int temp = dp[key()];
			update(i, true, -1 );
			if( temp + 1 <= dp[cur] )
			{
				update( i, false );
				temp = max( temp, dp[key()] );
				update( i, false, -1 );
				if( dp[cur] == temp + 1 )	// znalezlismy najlepsze pytanie
				{
					question = i;
					break;
				}
			}
		}
		update( question, mniejszaNiz( (int)question ) );
	}
	int answ = 0;
	while ( ++answ <= n  &&  state[ (unsigned)answ ] > k ) 
		;
 	odpowiedz( answ );
	state.assign( state.size(), 0 ); // przygotowujemy state[] i not_pos do nastepnej gry
	not_pos = 0;
}

int main()
{
	dajParametry( n, k, g );
	not_pos = 0;
	eval( 0 );
	while(g--) solve();
}