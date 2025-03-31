# Dynamiczny ciąg

## Opis zadania

Zaimplementuj strukturę danych, która wykonuje podstawowe operacje na ciągu liczb, w którym dopuszczamy wstawianie elementów. Elementy ciągu numerujemy od 0. Początkowo ciąg jest pusty. Dozwolone są następujące operacje:

- **insert(j,x,k)** – wstawia `k` egzemplarzy elementu `x` tuż przed `j`-tą pozycją ciągu, przesuwając elementy znajdujące się na pozycjach `j` oraz dalszych o `k` pozycji w prawo;
- **get(j)** – zwraca wartość `j`-tego elementu ciągu.

Możesz założyć, że w przypadku operacji **insert** zachodzi `0 ≤ j ≤ n`, a w przypadku operacji **get** `0 ≤ j < n`, przy czym `n` oznacza długość ciągu w momencie wykonywania operacji.

W tym zadaniu format wejścia jest zakamuflowany tak, by wymuszać rozwiązania działające on-line. W swoim rozwiązaniu możesz posłużyć się gotową implementacją zrównoważonych drzew binarnych z Internetu.

## Wejście

W pierwszym wierszu znajduje się liczba całkowita `m` `(1 ≤ m ≤ 500 000)`, oznaczająca liczbę operacji.

Każdy z kolejnych `m` wierszy zawiera jedną małą literę oznaczającą typ operacji:

- **i** dla **insert**,
- **g** dla **get**

po której następują liczby oddzielone odstępami:
- `j'`, `x` oraz `k` dla **insert**,
- `j'` dla **get**.

Parametr `j` w danym zapytaniu jest wyznaczany na podstawie wczytanej liczby `j'` według wzoru:

- `j = (j' + w) mod (n + 1)` w przypadku operacji **insert**,
- `j = (j' + w) mod n` w przypadku operacji **get**,

przy czym `w` to wynik ostatniej operacji **get** lub `0`, jeśli nie było wcześniej żadnej operacji **get**, a `n` to długość ciągu w momencie wykonywania operacji.

Liczby `j'` oraz `x` będą nieujemne i nie większe niż `10^9`, a liczby `k` będą z zakresu od `1` do `1000`.

## Wyjście

Dla każdej operacji **get** Twój program powinien wypisać jeden wiersz zawierający wynik. Możesz założyć, że zostanie zawsze wykonana co najmniej jedna operacja tego typu.

