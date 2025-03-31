# Arytmetyka przybliżonych wartości

Tam, gdzie dokonujemy pomiarów wielkości fizycznych, wyniki są obarczone pewnym błędem, np. `5m ± 10%`. Każdą taką przybliżoną wartość traktujemy jak zbiór możliwych wartości.  

Zaimplementuj pakiet operacji arytmetycznych na takich przybliżonych wartościach zgodny z załączonym interfejsem `ary.h`.  

## Wymagania pakietu

Pakiet ma zawierać:

### Definicja typu
- `struct wartosc`

### Konstruktory
- `wartosc_dokladnosc(x, p)` – zwraca `x ± p%` (dla `p > 0`)
- `wartosc_od_do(x, y)` – zwraca `(x+y)/2 ± (y-x)/2` (dla `x < y`)
- `wartosc_dokladna(x)` – zwraca `x ± 0`

### Selektory
- `in_wartosc(x, y)` – wartość `x` może być równa `y`
- `min_wartosc(x)` – kres dolny możliwych wartości `x`
  - `-\infty` jeśli `x` nie jest ograniczone od dołu  
  - `nan` jeśli `x` jest puste  
- `max_wartosc(x)` – kres górny możliwych wartości `x`
  - `\infty` jeśli `x` nie jest ograniczone od góry  
  - `nan` jeśli `x` jest puste  
- `sr_wartosc(x)` – średnia (arytmetyczna) wartości `min_wartosc(x)` i `max_wartosc(x)`
  - `nan` jeśli `min_wartosc(x)` i `max_wartosc(x)` nie są skończone  

### Modyfikatory
- `plus(a, b) = { x + y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`
- `minus(a, b) = { x - y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`
- `razy(a, b) = { x · y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`
- `podzielic(a, b) = { x / y : in_wartosc(a, x) ∧ in_wartosc(b, y) }`

## Założenia dotyczące liczb zmiennopozycyjnych
- Wszystkie argumenty typu `double` są liczbami rzeczywistymi (tj. różnymi od `HUGE_VAL`, `-HUGE_VAL` i `NAN`).
- Jeśli wynik nie jest liczbą rzeczywistą, powinien być odpowiednią z wartości: `HUGE_VAL`, `-HUGE_VAL` lub `NAN`.

## Zasady ułatwiające rozumowanie

1. **Domykanie wynikowych zbiorów wartości**  
   - Jeśli wynikiem operacji jest przedział otwarty, to zostaje on zamieniony na przedział domknięty.  

2. **Monotoniczność operacji**  
   - Jeśli wartości przybliżone `x`, `y` i `z` spełniają `x ⊆ y`, to:
     - `plus(x, z) ⊆ plus(y, z)`
     - `plus(z, x) ⊆ plus(z, y)`
     - I podobnie dla innych operacji arytmetycznych.

