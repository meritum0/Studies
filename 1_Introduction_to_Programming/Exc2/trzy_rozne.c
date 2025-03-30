/* Code review (Igor Holka)

    Kwestie techniczne:
    1. Tablice par trzy_do... tak naprawdę nie są konieczne–wystarczy trzymać w nich same indeksy, co mogłoby zwiększyć czytelność kodu.
    2. W tym voidzie od tych tablic nie trzeba wydzielać kolejnego ifa w funkcji– można po znalezieniu trzeciej po prostu zrobić break.(oczywiście to kwestia gustu)
    3. W zasadzie A, B, C to zbiór więc nie ma to większego znaczenia praktycznego, ale być może przy liczniku sieci zamiana pierwszej sieci na druga, a druga na trzecia, byłaby nieco oczywistsza do interpretacji.
    4. Przypisanie trzecia_siec = 0 nie jest potrzebne.
    5. Teoretycznie nie trzeba wszedzie castowac tych uintow(i zamieniac uintow na inty)-- mozna jedynie w mallocach itd. zcastowac inta na uinta tak by flagi sie nie denerwowaly: int n;
    scanf("%d", &n);
    uint nn = (uint) n;
    int *sieci = (int*)calloc(nn,sizeof(int));
    int *dist = (int*)calloc(nn,sizeof(int));
    cos takiego powoduje ze dalej jest mniej tych castow(kod wyglada “naturalniej”).

    Kwestie Stylu:
    Naprawde ciezko sie do czegos przyczepic.


    Odpowiedź (Aleksander Lukosek): zastosowałem się do punktów 1, 4 i pośrednio do 2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned int uint;

inline static uint min( uint a, uint b ) { return a < b ? a : b; }
inline static uint max( uint a, uint b ) { return a > b ? a : b; }

uint najdalsza( uint odleglosc[], uint sieci[], uint liczba_moteli )
{
    uint trzy_od_przodu[3] = { 0 };     // indeksy pierwszych trzech, różnych sieci od przodu...
    uint trzy_od_tylu[3] = { liczba_moteli-1, 0, 0};            // ...i od tyłu
    uint max_sum = 0;
    
    // znajdujemy indeksy pierwszych trzech, parami różnych sieci od przodu
    for( uint i = 1;  i < liczba_moteli && trzy_od_przodu[2] == 0;  i++ )
    {
        if( trzy_od_przodu[1] == 0  &&  sieci[i] != sieci[trzy_od_przodu[0]] )
            trzy_od_przodu[1] = i;
        else if( sieci[i] != sieci[trzy_od_przodu[0]]  &&  sieci[i] != sieci[trzy_od_przodu[1]] )
            trzy_od_przodu[2] = i;
    }
    if( trzy_od_przodu[2] == 0 ) return 0;	//Na autostradzie nie ma przynajmniej 3 różnych sieci
    
    // znajdujemy indeksy pierwszych trzech, parami różnych sieci od tyłu
    for( int i = (int)liczba_moteli - 2;  i >= 0 && trzy_od_tylu[2] == 0;  i-- )
    {
        if( trzy_od_tylu[1] == 0  &&  sieci[i] != sieci[trzy_od_tylu[0]] )
            trzy_od_tylu[1] = (uint)i;
        else if( sieci[i] != sieci[trzy_od_tylu[0]]  &&  sieci[i] != sieci[trzy_od_tylu[1]] )
            trzy_od_tylu[2] = (uint)i;
    }
    
    for( int n = 0; n < 3; n++ )
        for( int m = 0; m < 3; m++ )
            if( sieci[trzy_od_przodu[n]] != sieci[trzy_od_tylu[m]] )  // jeśli skrajne są różne...
                //...przelatujemy po wszystkich sieciach pomiędzy nimi i wybieramy "najdalszą" trójkę
                for( uint i = trzy_od_przodu[n] + 1; i < trzy_od_tylu[m]; i++ )
                    if( sieci[i] != sieci[trzy_od_przodu[n]]  &&  sieci[i] != sieci[trzy_od_tylu[m]] )
                        max_sum = max( max_sum, min( odleglosc[i] - odleglosc[ trzy_od_przodu[n] ], 
                                                     odleglosc[ trzy_od_tylu[m] ] - odleglosc[i] ) );
    
    return max_sum;
}

uint najblizsza( uint odleglosc[], uint sieci[], uint liczba_moteli )
{
    /* Algorytm opiera się na tym, że pomiędzy skrajnymi sieciami nie może się pojawić 
       motel którejś ze skrajnych sieci, bo wtedy istnieje wewnątrz aktualnej trójki
       pewna "niedalsza" trójka.
       trzecia_siec przechowuje prawą skrajną sieć z trójki */
    uint pierwsza_siec = sieci[0], druga_siec = 0, trzecia_siec = 0;
    uint licznik_sieci[2] = { 1, 0 };  // wystąpienia pierwsza_siec i druga_siec w danym fragmencie
    uint min_sum = UINT_MAX;           // przechowuje wynik najbliższej trójki do tej pory
    
    for( uint left=0, right=1; right < liczba_moteli; right++ ) // left i right to krańce gąsienicy
    {
        if( druga_siec == 0  &&  sieci[right] != pierwsza_siec )
            druga_siec = sieci[right];      // trafiliśmy na pierwszą sieć różną od pierwszej
        if( sieci[right] == pierwsza_siec )
        {
            licznik_sieci[0]++;
            continue;
        }
        if( sieci[right] == druga_siec )
        {
            licznik_sieci[1]++;
            continue;
        }
        trzecia_siec = sieci[right];		// trafiliśmy na sieć różną od pierwszej i drugiej

        // left przesuwamy do momentu aż pierwsza lub druga siec wystepuje tylko raz we fragmencie
        for(; left < right; left++ )
        {
            if( sieci[left] == pierwsza_siec )
                licznik_sieci[0]--;
            else if( sieci[left] == druga_siec )
                licznik_sieci[1]--;
            if( licznik_sieci[0] == 0 || licznik_sieci[1] == 0 )
                break;
        }
            
        // przelatujemy po wszystkich elementach w środku wybieramy "najbliższy" wynik
        for( uint j = left + 1; j < right; j++ )
            min_sum = min( min_sum, max( odleglosc[j] - odleglosc[left], odleglosc[right] - odleglosc[j] ) );
            
        left = right - 1;	//Kolejna gąsienica może startować od tego miejsca
        if( licznik_sieci[0] == 0 )
            pierwsza_siec = trzecia_siec;
        if( licznik_sieci[1] == 0 )
            druga_siec = trzecia_siec;
        licznik_sieci[0] = licznik_sieci[1] = 1;    //Mamy teraz po jednym wystapieniu dwoch wartosci
    }
    if( min_sum == UINT_MAX )	return 0;      //Na autostradzie jest mniej niż 3 różne sieci moteli
    
    return min_sum;
}

int main( int argc  __unused , char *argv[] __unused )
{
    uint liczba_moteli;
    scanf("%u", &liczba_moteli);
    uint *sieci = (uint*)malloc( liczba_moteli * sizeof(uint) );
    uint *odleglosc = (uint*)malloc( liczba_moteli * sizeof(uint) );
    for( uint i = 0; i < liczba_moteli; i++ )
        scanf("%u %u", sieci + i, odleglosc + i );
    printf("%u %u\n", najblizsza( odleglosc, sieci, liczba_moteli ),
                      najdalsza( odleglosc, sieci, liczba_moteli) );
    
    free(sieci);
    free(odleglosc);
    
    return 0;
}
