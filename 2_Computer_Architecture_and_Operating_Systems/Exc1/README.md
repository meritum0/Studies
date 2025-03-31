# Bramki NAND

## Opis Zadania
Zadanie polega na zaimplementowaniu w języku C dynamicznie ładowanej biblioteki obsługującej kombinacyjne układy boolowskie złożone z bramek NAND.

## Opis Bramek NAND
- Bramka NAND ma całkowitą nieujemną liczbę wejść i jedno wyjście.
- Bramka NAND bez wejść daje na wyjściu zawsze sygnał o wartości `false`.
- Bramka NAND o jednym wejściu jest negacją.
- Na wejście bramki można podać sygnał boolowski przyjmujący wartości `false` lub `true`.
- Na wyjściu bramka daje sygnał `false`, jeśli na wszystkich jej wejściach są sygnały `true`, a `true` w przeciwnym przypadku.
- Sygnał z wyjścia bramki można podłączyć do wielu wejść bramek.
- Do wejścia bramki można podłączyć tylko jedno źródło sygnału.

## Interfejs Biblioteki
Interfejs biblioteki znajduje się w pliku `nand.h`, który zawiera poniższe deklaracje:

```c
typedef struct nand nand_t;
```
Typ ten trzeba zdefiniować w ramach zadania.

```c
nand_t * nand_new(unsigned n);
```
Tworzy nową bramkę NAND o `n` wejściach.
- Zwraca wskaźnik na strukturę reprezentującą bramkę NAND.
- Zwraca `NULL`, jeśli wystąpił błąd alokacji pamięci (`errno` = `ENOMEM`).

```c
void nand_delete(nand_t *g);
```
Usuwa wskazaną bramkę i zwalnia używaną przez nią pamięć.

```c
int nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k);
```
Podłącza wyjście bramki `g_out` do wejścia `k` bramki `g_in`.
- Zwraca `0` w przypadku sukcesu.
- Zwraca `-1` w przypadku błędu (`errno` = `EINVAL` lub `ENOMEM`).

```c
int nand_connect_signal(bool const *s, nand_t *g, unsigned k);
```
Podłącza sygnał boolowski `s` do wejścia `k` bramki `g`.
- Zwraca `0` w przypadku sukcesu.
- Zwraca `-1` w przypadku błędu (`errno` = `EINVAL` lub `ENOMEM`).

```c
ssize_t nand_evaluate(nand_t **g, bool *s, size_t m);
```
Wyznacza wartości sygnałów na wyjściach podanych bramek i oblicza długość ścieżki krytycznej.

```c
ssize_t nand_fan_out(nand_t const *g);
```
Wyznacza liczbę wejść bramek podłączonych do wyjścia danej bramki.

```c
void* nand_input(nand_t const *g, unsigned k);
```
Zwraca wskaźnik do sygnału boolowskiego lub bramki podłączonej do wejścia `k` bramki `g`.

```c
nand_t* nand_output(nand_t const *g, ssize_t k);
```
Pozwala iterować po bramkach podłączonych do wyjścia podanej bramki.

## Wymagania Funkcjonalne
- Wyznaczenie wartości sygnału i długości ścieżki krytycznej na wyjściu bramki wymaga rekurencyjnego wyznaczenia tych wartości na jej wejściach.
- Procedura musi obsługiwać przypadki błędów, takie jak brak podłączenia sygnału do wejścia lub cykle w połączeniach bramek.

Wycieki pamięci będą sprawdzane za pomocą valgrinda

---