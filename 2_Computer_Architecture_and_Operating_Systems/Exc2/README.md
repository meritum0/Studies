# Dzielenie

## Opis Zadania
Zaimplementuj w asemblerze funkcję `mdiv`, wywoływaną z języka C:

```c
int64_t mdiv(int64_t *x, size_t n, int64_t y);
```

Funkcja wykonuje dzielenie całkowitoliczbowe z resztą.

## Opis Działania
- Pierwszy i drugi parametr (`x` i `n`) określają dzielną.
  - `x` to wskaźnik na tablicę `n` liczb 64-bitowych.
  - Dzielna ma `64 * n` bitów i jest zapisana w porządku `little-endian`.
- Trzeci parametr `y` to dzielnik.
- Wynikiem funkcji jest **reszta z dzielenia** `x` przez `y`.
- **Iloraz jest zapisywany w tablicy `x`**.

## Obsługa Nadmiaru (Overflow)
- Jeśli iloraz nie mieści się w tablicy `x`, występuje nadmiar (`overflow`).
- Dzielenie przez zero także powoduje nadmiar.
- Funkcja reaguje na nadmiar tak jak instrukcje `div` i `idiv` – zgłasza **przerwanie numer 0**.
- W systemie Linux skutkuje to sygnałem `SIGFPE`.

## Założenia
- `x` jest poprawnym wskaźnikiem.
- `n` jest liczbą dodatnią.

## Kompilowanie Rozwiązania
Rozwiązanie będzie kompilowane poleceniem:

```sh
nasm -f elf64 -w+all -w+error -o mdiv.o mdiv.asm