/* Autor kodu: Aleksander Lukosek
Review Poprzedni w zakresie (Gracjan Barski):
1. Mała objętość kodu, na plus. Użycie makra zwieksza czytelnosc kodu.
2. Funkcja pushBack() wywoła undefined behaviour jeśli vector roots będzie pusty, a może tak być, jeśli vector X podany do init() będzie pusty, dodałbym sprawdzenie dla bezpieczeństwa.
3. Plus za zwięzła funkcję build()
4. Program przechodzi testy w dużej ilości, bez wycieków pamięci 
*/

#include "prev.h"
#include <memory>
#include <climits>
#define MID(l,r) (int)((l) + ((long)(r) - (long)(l))/2)

using namespace std;

typedef shared_ptr<struct _node> tree;
typedef struct _node
{
    int   val;
    tree  left, right;
    _node( int v, tree l, tree r ) : val(v), left(l), right(r) {}; 
} node;

vector<tree> roots; //przechowuje korzenie kolejnych wersji drzewa przedzialowego

tree build( tree prev, int nr, int idx, int l, int r )  //nr to wartosc dodanego elementu
{
    if ( l == r )  return make_shared<node>( idx, nullptr, nullptr );

    tree left, right;
    if ( MID(l,r) >= nr )
    {
        right = prev ? prev->right : nullptr;   //podpinam czesc poprzedniej wersji
        left = build( prev ? prev->left : nullptr, nr, idx, l, MID(l,r) );
    }
    else 
    {
        left = prev ? prev->left : nullptr;    //podpinam czesc poprzedniej wersji
        right = build( prev ? prev->right : nullptr, nr, idx, MID(l,r) + 1, r );
    }
    return make_shared<node>( max( right ? right->val : -1, left ? left->val : -1), left, right );
}

int search( tree t, int lo, int hi, int l, int r )
{
    if ( !t  ||  r < lo  ||  hi < l )  return -1;   //przedzialy sa rozlaczne
    if ( lo <= l  &&  hi >= r )  return t->val;     //przedzial drzew zawarty w calosci w [lo,hi]
    return max( search( t->right, lo, hi, MID(l,r)+1, r), search( t->left, lo, hi, l, MID(l,r) ) );
}

void pushBack( int value )
{
    if( roots.size() == 0 )
        init( vector<int>{value} );
    else
        roots.push_back( build( roots.back(), value, (int)roots.size(), INT_MIN, INT_MAX ));
}

void init( const vector<int> &seq )
{
    for( unsigned long i = 0; i < seq.size(); i++ )
        roots.push_back( build(roots.empty() ? NULL : roots.back(), seq[i], (int)i, INT_MIN, INT_MAX) );
}

int prevInRange( int i, int lo, int hi )
{
    return search( roots[ (unsigned long)i ], lo, hi, INT_MIN, INT_MAX );
}

void done()
{
    roots.clear();
}
