# Bazarek

## Opis Problemu

Mały Bajtek spędza wakacje u babci Bajtuli, która codziennie rano udaje się na bazarek po zakupy. Bajtek zauważył, że każdego dnia babcia wydaje na zakupy kwotę będącą nieparzystą liczbą całkowitą. Wkrótce ustalił, że jest to cecha wszystkich bajtockich babć.

Babcia Bajtula kupuje maksymalnie jeden egzemplarz każdego z dostępnych produktów i nie chce zabierać zbyt dużej sumy pieniędzy. Pewnego dnia poprosiła Bajtka o pomoc w ustaleniu, ile pieniędzy musi zabrać, jeśli chce kupić dokładnie `k` produktów. Bajtek, nie wiedząc, które produkty babcia zamierza kupić, musi znaleźć minimalną kwotę wystarczającą na dowolne `k` produktów, tak aby suma ich kosztów była nieparzysta.

Chcąc rozwiązać problem systematycznie, Bajtek postanowił napisać program, który na podstawie cen wszystkich produktów dostępnych na bazarku będzie odpowiadał na pytania babci.

## Wejście

1. Pierwszy wiersz wejścia zawiera jedną liczbę całkowitą `n` (`1 ≤ n ≤ 1 000 000`), oznaczającą liczbę produktów dostępnych na bazarku.
2. Drugi wiersz zawiera `n` liczb całkowitych z przedziału `[1, 10^9]`, oznaczających ceny poszczególnych produktów. Liczby te są podane w kolejności niemalejącej.
3. Trzeci wiersz zawiera jedną liczbę całkowitą `m` (`1 ≤ m ≤ 1 000 000`), oznaczającą liczbę dni, które Bajtek spędzi jeszcze u babci.
4. Każdy z kolejnych `m` wierszy zawiera jedną liczbę całkowitą `k_i` (`1 ≤ k_i ≤ n`), oznaczającą liczbę produktów, które babcia zamierza kupić danego dnia.

## Wyjście

Program powinien wypisać `m` wierszy, gdzie:

- `i`-ty wiersz (dla `i = 1, ..., m`) zawiera jedną liczbę całkowitą, oznaczającą maksymalną nieparzystą cenę `k_i` produktów.
- Jeśli nie da się wybrać `k_i` produktów, których łączna cena byłaby nieparzysta, w `i`-tym wierszu powinna znaleźć się liczba `-1`.
