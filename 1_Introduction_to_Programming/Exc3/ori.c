/* Autor kodu: Aleksander Lukosek
   Review: Gracjan Barski
1. Czytelnie napisany kod; oddzielone od siebie nawiasy, równo wyindentowane warunki logiczne,
   konsystentny styl kodu utrzymany na przestrzeni całego pliku.
2. Ładne użycie "union"
3. Ładne wczytanie danych
4. Na plus użycie define do EPS
5. Brak 'free'
6. Może to nieistotny techniczny szczegół, ale zmieniłbym typ zwracanej wartości funkcji 'ile_wartsw' 
   z 'int' na 'long', ponieważ w przypadku gdy kartka zostałaby złożona więcej niż 32 razy,
   to wynik mógłby nie zmieścić się w typie 'int'

Odpowiedź na review: Zastosowałem się do punktu 6.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define EPS 1e-10
typedef unsigned int uint;

typedef struct _point
{
    double x, y;
} point;

typedef struct _kartka
{ 
    char 	znak;
    union
    {
        struct _rect
        {
            point  a, b;
        } rect;	
        struct _circle
        {
            point   c;
            double  r;
        } circle;
        struct _line
        {
            point p1, p2;
            uint	  parent;
        } line;
    };
} kartka;

bool is_equal( double a, double b )   { return fabs(a - b) < EPS; }
bool is_ge( double a, double b)       { return (a > b) || is_equal(a, b); }

double iloczyn_wektorowy( point a, struct _line linia )
{ 
    return (linia.p2.x - linia.p1.x) * (a.y - linia.p1.y)  -
           (linia.p2.y - linia.p1.y) * (a.x - linia.p1.x); 
}

bool w_prostokacie( struct _rect prostokat, point punkt ) 	// czy punkt nalezy do prostokata
{
    return is_ge( punkt.x, prostokat.a.x )  &&  is_ge( prostokat.b.x, punkt.x )  &&
           is_ge( punkt.y, prostokat.a.y )  &&  is_ge( prostokat.b.y, punkt.y );
}

bool w_kolku( struct _circle kolko, point punkt ) 		// czy punkt należy do kółka
{
    return is_ge( kolko.r, (double)sqrt( pow( fabs(kolko.c.x - punkt.x), 2 )
           + pow( fabs(kolko.c.y - punkt.y), 2 ) ) );
}

point odbicie( struct _line linia, point punkt )
{
    double a, b, d;	// wspolczynniki prostej y = ax + b
  
    if( is_equal( linia.p1.x, linia.p2.x ) )		// prosta rownolegla do osi OY
       return (point){ (linia.p1.x - punkt.x) * 2 + punkt.x, punkt.y };
  
    a = (linia.p1.y - linia.p2.y) / (linia.p1.x - linia.p2.x);
    b = linia.p2.y - a * linia.p2.x;
    d = (punkt.x + ( punkt.y - b) * a)/(1 + a*a); // projekcja (.x) punktu na prostą
  
    return (point){ 2*d - punkt.x, 2*d*a - punkt.y + 2*b }; 
}

long ile_warstw( kartka *kartki, uint nr, point punkt )
{
    if( kartki[--nr].znak == 'P' )					// prostokąt
        return w_prostokacie( kartki[nr].rect, punkt);
    if( kartki[nr].znak == 'K' )					// kółko
        return w_kolku( kartki[nr].circle, punkt);
    if( iloczyn_wektorowy( punkt, kartki[nr].line ) < -EPS )
        return 0;	// punkt leży po prawej stronie prostej wzdłuż ktorej zginaliśmy kartkę
    if( is_equal( iloczyn_wektorowy( punkt, kartki[nr].line ), 0 ) )
        return ile_warstw( kartki, kartki[nr].line.parent, punkt ); // punkt leży na tej prostej
    
    // punkt po lewej: suma warstw przed złożeniem w danym punkcie i punkcie odbitym (nałożonym)
    return ile_warstw( kartki, kartki[nr].line.parent, punkt ) +
           ile_warstw( kartki, kartki[nr].line.parent, odbicie( kartki[nr].line, punkt ) );
}

int main( int argc __attribute__((unused)), char *argv[] __attribute__((unused)) )
{
    uint    liczba_kartek, liczba_pytan, ktora_kartka;
    
    scanf( "%u %u", &liczba_kartek, &liczba_pytan );
    
    kartka *kartki = (kartka*)malloc( liczba_kartek * sizeof(kartka) );
    for( int i = 0; i < (int)liczba_kartek; i++ )
    {
        scanf( " %c", &kartki[i].znak );

        if( kartki[i].znak == 'P' )                                     // prostokąt
            scanf( "%lf %lf %lf %lf", &kartki[i].rect.a.x, &kartki[i].rect.a.y, 
                                      &kartki[i].rect.b.x, &kartki[i].rect.b.y );
        else if( kartki[i].znak == 'K' )                                // kółko
            scanf( "%lf %lf %lf", &kartki[i].circle.c.x, &kartki[i].circle.c.y, 
                                  &kartki[i].circle.r );
        else if( kartki[i].znak == 'Z')                                 // złożenie
            scanf( "%u %lf %lf %lf %lf", &kartki[i].line.parent, 
                                         &kartki[i].line.p1.x, &kartki[i].line.p1.y, 
                                         &kartki[i].line.p2.x, &kartki[i].line.p2.y );
    }
    
    point    punkt;
    for( int i = 0; i < (int)liczba_pytan; i++ )
    {
        scanf( "%u %lf %lf\n", &ktora_kartka, &punkt.x, &punkt.y );
        printf( "%li\n", ile_warstw( kartki, ktora_kartka, punkt) );
    }
}
