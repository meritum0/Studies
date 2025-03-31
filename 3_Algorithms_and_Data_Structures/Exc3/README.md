# Sortowanie Komórkowe

## Opis Problemu

Sortowanie komórkowe to algorytm o stosunkowo dużej złożoności czasowej, który działa krokowo. Wykonuje on sekwencję operacji na danym ciągu, aż stanie się on posortowany niemalejąco.

Każdy krok algorytmu działa w następujący sposób:

1. Przeglądamy ciąg od lewej do prawej.
2. Budujemy nowy, wynikowy ciąg pomocniczy:
   - Pierwszy element oryginalnego ciągu umieszczamy na początku ciągu wynikowego.
   - Każdy kolejny element umieszczamy na **początku**, jeśli poprzedni element był większy.
   - Każdy kolejny element umieszczamy na **końcu**, jeśli poprzedni element był mniejszy.

### Przykład

Dla ciągu `5, 6, 2, 1, 4, 3`, kolejne etapy budowania ciągu wynikowego wyglądają tak:

- `5`
- `5, 6`
- `2, 5, 6`
- `1, 2, 5, 6`
- `1, 2, 5, 6, 4`
- `3, 1, 2, 5, 6, 4`

Ostateczny wynik kroku sortowania to: `3, 1, 2, 5, 6, 4`.

Twoim zadaniem jest określenie, ile różnych ciągów mogło przejść w **jednym kroku** algorytmu do zadanego ciągu wynikowego.

## Wejście

- Pierwsza linia zawiera jedną liczbę całkowitą `n` (`1 ≤ n ≤ 1000`), oznaczającą długość ciągu.
- Druga linia zawiera ciąg `n` parami różnych liczb całkowitych ze zbioru `{1, ..., n}`, reprezentujących ciąg, który należy odsortować.

## Wyjście

Należy wypisać **resztę z dzielenia przez** `10^9` liczby różnych ciągów, które w **jednym kroku** sortowania komórkowego mogły przejść na dany ciąg wynikowy.
