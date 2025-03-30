/*  Autor kodu:  Aleksander Lukosek
    Peer review: Grzegorz Krawczyk */
/*
    1. iszero powinno użyć 1e-10 - tak było napisane w treści zadania
    2. Czasem nieścisłe "spacjowanie" (m. in. linijki 29, 58, 69, 80, 119, 146, 216, 225, 249, 251, 258)
    3. Bardzo... rozbudowane mnożenie - czy aby na pewno każdy z tych przypadków wymaga oddzielnego rozpatrzenia?
    4. Istnieje coś takiego jak fmax i fmin - polecam użyć
    5. Ostatnie 2 returny w plus zwracają to samo
*/
    
/* Zastosowałem się do wszystkich wskazówek z review */

#include "ary.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#define wartosc_NAN     ((wartosc){ .left = NAN, .right = NAN})         /* compound literals and */
#define wartosc_HUGE    ((wartosc){-HUGE_VAL,HUGE_VAL})     /* designated initializers since C99 */

bool    iszero( double x )          { return fabs(x) < 1e-10; }
bool    is_ge( double x, double y ) { return x > y || iszero( fabs(x - y) ); }         //  x >= y ?
bool    is_gt( double x, double y ) { return x > y && !iszero( fabs(x - y) ); }        //   x > y ?
bool    czy_antyzbior( wartosc a )  { return is_gt( a.left, a.right ); }     //  <==>  left > right
bool    is_nan( wartosc w )         { return isnan(w.left) || isnan(w.right); }

wartosc odwrotnosc( wartosc a )
{
    if ( ( iszero(a.left) && iszero(a.right) ) || is_nan(a) )
        return wartosc_NAN;
    if ( isinf(a.left) && isinf(a.right) )
        return wartosc_HUGE;
    if ( iszero(a.left) && is_gt(a.right, 0) )         /* [0, x+] */
        return (wartosc){ 1 / a.right, HUGE_VAL };
    if ( is_gt(0, a.left) && iszero(a.right) )         /* [-x,0] */
        return (wartosc){ -HUGE_VAL, 1 / a.left };
    if ( is_gt(a.left, 0) && iszero(a.right) )         /* [-INF, 0]U[x+, INF] */
        return (wartosc){ -HUGE_VAL, 1 / a.left };
    if ( iszero(a.left) && is_gt(0, a.right)  )        /* [-INF, -x]U[0, INF] */
        return (wartosc){ 1 / a.right, HUGE_VAL };
    /* Oba końce w "a" są ujemne, dodatnie, albo różnych znaków (w tym przypadek antyzbioru) */
    return (wartosc){ 1 / a.right, 1 / a.left };
}

/************************  implementacja interfejsu ary.h  ********************************/
wartosc wartosc_dokladnosc( double x, double p )
{
    p *= fabs(x) / 100.0;
    return (wartosc){ x - p,  x + p };
}

wartosc wartosc_od_do( double x, double y )
{
    return (wartosc){ .left = x, .right = y };
}

wartosc wartosc_dokladna( double x )
{
    return (wartosc){ x, x };
}

bool in_wartosc( wartosc w, double x )
{
    if ( is_nan(w) ) return false;
    
    if( is_ge( w.right, w.left) )                   /* Przypadek zwykłego zbioru */
        return is_ge( x, w.left )  &&  is_ge( w.right, x );
    else                                            /* Przypadek antyzbioru */
        return is_ge( w.right, x )  ||  is_ge( x, w.left );
}

double min_wartosc( wartosc w )
{
    if ( is_nan(w) ) return NAN; 
    
    if ( czy_antyzbior(w) )  return -HUGE_VAL;
    
    return w.left;
}

double max_wartosc( wartosc w )
{
    if ( is_nan(w) ) return NAN;
    
    if ( czy_antyzbior(w) ) return HUGE_VAL;
    
    return w.right; 
}

double sr_wartosc( wartosc w )
{
    if ( is_nan(w) ) return NAN;
    
    /* może być różnie określona dla zbiorów zawierających -INF i INF */
    if ( ( isinf(w.left) && isinf(w.right) ) || czy_antyzbior(w) ) return NAN;
    
    return (w.left + w.right) / 2.0;
}

wartosc plus( wartosc a, wartosc b )
{
    if ( is_nan(a) || is_nan(b) ) return wartosc_NAN;
    
    if ( czy_antyzbior(a) && czy_antyzbior(b) ) return wartosc_HUGE;     // a i b są antyzbiorami
    
    if ( ( czy_antyzbior(a) || czy_antyzbior(b) )  &&            // a lub b jest antyzbiorem ORAZ
         is_ge( a.right + b.right, a.left + b.left ) ) // po dodaniu "zamyka się" dziura w środku
    {
        return wartosc_HUGE;
    }
  
    return (wartosc){ a.left + b.left, a.right + b.right };
}

wartosc minus( wartosc a, wartosc b )
{
    return plus( a, (wartosc){ -b.right, -b.left } );                 // a + przeciwna(b)
}

wartosc razy( wartosc a, wartosc b ) 
{
    if ( is_nan(a) || is_nan(b) ) return wartosc_NAN;
    
    // jesli a lub b są zerem
    if( ( iszero(a.left) && iszero(a.right) ) ||
        ( iszero(b.left) && iszero(b.right) ) ) 
        return (wartosc){ 0, 0 };
  
    if ( czy_antyzbior(a) && czy_antyzbior(b) )
    {
        // ktoryś z nich zawiera 0
        if ( in_wartosc(a, 0) || in_wartosc(b, 0) ) return wartosc_HUGE;
        
        // żaden nie zawiera 0
        double left = fmax( a.left * b.right, a.right * b.left );  // maksimum lewej części
        double right = fmin( a.left * b.left, a.right * b.right ); // minimum prawej części
        return is_ge( right, left ) ? (wartosc){ right, left } : wartosc_HUGE;
    }
    else if ( czy_antyzbior(a) )
    {
        wartosc x = razy( (wartosc){ -HUGE_VAL, a.right }, b );
        wartosc y = razy( (wartosc){ a.left, HUGE_VAL   }, b );

        if ( isinf(x.left) && isinf(y.right) )
        {
          if ( is_ge( x.right, y.left ) ) return wartosc_HUGE;
          
          return (wartosc){ y.left, x.right };
        }
        else
        {
          if ( is_ge( y.right, x.left ) ) return wartosc_HUGE;
          
          return (wartosc){ x.left, y.right };
        }
    }
    else if ( czy_antyzbior(b) ) 
        return razy( b, a );
  
    /* Działanie na zwykłych zbiorach. Wymnażamy wszystkie pary i bierzemy min/max */
    double left = fmin( fmin( a.left * b.left, a.left * b.right ),
                        fmin( a.right * b.left, a.right * b.right ) );
                    
    double right = fmax( fmax( a.left * b.left, a.left * b.right ),
                         fmax( a.right * b.left, a.right * b.right ) );
    return (wartosc){ left, right };
}

wartosc podzielic( wartosc a, wartosc b )
{
    return razy( a, odwrotnosc(b) );
}
