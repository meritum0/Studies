# Badanie liczby warstw w origami

## Opis problemu

Napisz program dla fanów origami do badania, **ile warstw ma w danym punkcie sprytnie poskładana kartka papieru**.  
Program powinien wczytać **opis tworzenia kolejnych kartek origami** oraz **zapytania o liczbę warstw w zadanych punktach** i zwrócić odpowiedzi.

## Wejście

Program wczytuje z `stdin`:

1. Dwie liczby całkowite `n` i `q` – liczba kartek oraz liczba zapytań.
2. `n` wierszy opisujących kolejne kartki.
3. `q` zapytań o liczbę warstw w zadanych punktach.

### Opis kartki

Kartka `i` (`1 ≤ i ≤ n`) jest opisana w wierszu `i + 1` w jednej z trzech postaci:

- **Prostokąt:**  
  ```
  P x1 y1 x2 y2
  ```
  Domknięty prostokąt o bokach równoległych do osi układu współrzędnych:  
  - Lewy dolny róg: `(x1, y1)`
  - Prawy górny róg: `(x2, y2)`
  
  📌 Wbicie szpilki **wewnątrz** (lub na krawędzi) prostokąta → `1 warstwa`,  
  📌 W przeciwnym razie → `0 warstw`.

- **Koło:**  
  ```
  K x y r
  ```
  Koło domknięte o środku `(x, y)` i promieniu `r`.

- **Złożenie kartki:**  
  ```
  Z k x1 y1 x2 y2
  ```
  Kartka `i` powstała przez **złożenie** `k`-tej kartki (`1 ≤ k < i`) wzdłuż prostej przechodzącej przez punkty `(x1, y1)` i `(x2, y2)`.  
  - Punkt `(x1, y1)` i `(x2, y2)` muszą być różne.  
  - **Z prawej strony** prostej (patrząc w kierunku od `(x1, y1)` do `(x2, y2)`) papier jest przekładany **na lewą**.
  - 📌 **Przebicie po prawej stronie** → `0 warstw`
  - 📌 **Przebicie dokładnie na prostej** → tyle samo, co przed złożeniem
  - 📌 **Przebicie po lewej stronie** → suma warstw przed i po złożeniu

### Opis zapytania

Zapytanie `j` (`1 ≤ j ≤ q`) znajduje się w wierszu `n + j + 1` i ma postać:

```
k x y
```
- `k` – numer kartki (`1 ≤ k ≤ n`)
- `(x, y)` – punkt, w którym wbijamy szpilkę.

## Wyjście

Program wypisuje na `stdout` `q` wierszy – odpowiedzi na zapytania o liczbę warstw w podanych punktach.

## Ograniczenia

- `1 ≤ n, q ≤ 1 000 000`
- Wszystkie liczby wejściowe mieszczą się w zakresie `int`.
