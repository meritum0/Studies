# Domino

## Opis Problemu

Na prostokątnej planszy `k × n` każde pole zawiera liczbę całkowitą. Zadanie polega na rozmieszczeniu kamieni domina (`1 × 2`) na tej planszy w taki sposób, aby:

- Każdy kamień przykrywał dokładnie dwa pola.
- Żadne pole nie było przykryte przez więcej niż jeden kamień.
- Suma wartości na przykrytych polach była jak największa.

## Wejście

- Pierwsza linia zawiera dwie liczby całkowite `n` i `k` (`1 ≤ n ≤ 1000, 1 ≤ k ≤ 12`).
- Następne `k` linii zawiera `n` liczb całkowitych oddzielonych spacjami (`−10^6 ≤ a_{i,j} ≤ 10^6`), oznaczających wartości na poszczególnych polach planszy.

## Wyjście

- Jedna liczba całkowita – największa możliwa wartość rozmieszczenia kamieni domina.
