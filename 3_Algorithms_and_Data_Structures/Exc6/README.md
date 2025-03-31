# k-inwersje

## Opis Problemu

Niech `a1, ..., an` będzie permutacją liczb od `1` do `n`. **k-inwersją** w tej permutacji nazywamy ciąg indeksów `i1, i2, ..., ik`, taki że:

- `1 ≤ i1 < i2 < ... < ik ≤ n`
- `a_{i1} > a_{i2} > ... > a_{ik}`

Zadanie polega na policzeniu liczby takich **k-inwersji** w podanej permutacji.

## Wejście

- Pierwszy wiersz zawiera dwie liczby całkowite `n` oraz `k` (`1 ≤ n ≤ 20 000, 2 ≤ k ≤ 10`).
- Drugi wiersz zawiera permutację liczb `{1, ..., n}`.

## Wyjście

- Jedna liczba całkowita – reszta z dzielenia przez `10^9` liczby **k-inwersji** w podanej permutacji.
