# Obliczeniowa weryfikacja hipotezy kombinatorycznej

Dla danego multizbioru liczb naturalnych $A$ oznaczamy $\sum A=\sum_{a \in A} a$. Na przykład jeśli $A=\{1,2,2,2,10,10\}$, to $\sum A=27$. Dla dwóch multizbiorów piszemy $A \supseteq B$ jeśli każdy element występuje w $A$ co najmniej tyle razy, co w $B$.

Na cele zadania przyjmijmy następujące definicje.

## Definicje

**Definicja.** Multizbiór $A$ nazywamy *d-ograniczonym*, dla liczby naturalnej $d$, jeśli jest skończony i wszystkie jego elementy należą do $\{1,\dots,d\}$ (z dowolnymi powtórzeniami).

**Definicja.** Parę *d*-ograniczonych multizbiorów $(A,B)$ nazywamy *bezspornymi*, jeśli dla wszystkich $A' \subseteq A$ i $B' \subseteq B$ zachodzi:
$$\sum A' = \sum B' \iff A' = B' = \emptyset \lor (A' = A \land B' = B).$$
Innymi słowy $\sum A = \sum B$, ale poza tym sumy dowolnych niepustych podzbiorów $A$ i $B$ muszą się różnić.

## Problem

Dla ustalonego $d \geq 3$ (mniejszych $d$ nie będziemy rozważać) i multizbiorów $A_0,B_0$, chcemy znaleźć bezsporne *d*-ograniczone multizbiory $A \supseteq A_0$ i $B \supseteq B_0$, które maksymalizują wartość $\sum A$ (równoważnie $\sum B$). Oznaczmy tę wartość przez $\alpha(d,A_0,B_0)$. Przyjmijmy $\alpha(d,A_0,B_0)=0$ jeśli $A_0$ i $B_0$ nie są *d*-ograniczone lub nie mają *d*-ograniczonych bezspornych nadmultizbiorów.

## Przykłady

- $\alpha(d,\emptyset,\emptyset) \geq d(d-1)$
  - **Szkic dowodu:** Zbiory $A=\{d,\dots,d\}$ ($d-1$ razy) oraz $B=\{d-1,\dots,d-1\}$ ($d$ razy) spełniają warunki dla $\sum A=d(d-1)=\sum B$.
- $\alpha(d,\emptyset,\{1\}) \geq (d-1)^2$
  - **Szkic dowodu:** Zbiory $A=\{1,d,\dots,d\}$ ($d-2$ razy) oraz $B=\{d-1,\dots,d-1\}$ ($d-1$ razy) spełniają warunki dla $\sum A=1+d(d-2)=(d-1)^2=\sum B$.

Można udowodnić, że powyższe przykłady są najlepsze, tzn. $\alpha(d,\emptyset,\emptyset)=d(d-1)$ oraz $\alpha(d,\emptyset,\{1\})=(d-1)^2$.

Niemniej w tym zadaniu będziemy chcieli zweryfikować to obliczeniowo dla jak największych $d$, a także wyliczyć wartości $\alpha$ dla innych wymuszonych multizbiorów $A_0,B_0$.

## Rekurencja z nawrotami

Wartości $\alpha(d,A_0,B_0)$ możemy wyliczyć rekurencyjnie, budując inkrementacyjnie multizbiory $A \supseteq A_0$ i $B \supseteq B_0$. Oznaczmy przez $A_\Sigma=\{\sum A' : A' \subseteq A\}$, czyli zbiór wszystkich możliwych sum, jakie możemy uzyskać ze zbioru $A$.

*Tutaj był podany algorytm rekurencyjny w pseudokodzie.

W praktyce, aby nie liczyć zbiorów sum $A_\Sigma$ i $B_\Sigma$ za każdym razem od nowa, przekazujemy $A_\Sigma$ i $B_\Sigma$. Gdy dodajemy element $x$ do $A$, to nowy $A_\Sigma$ wynosi $A_\Sigma \cup (A_\Sigma + x)$.

## Zadanie

Zadanie polega na napisaniu dwóch innych implementacji tego samego obliczenia:
1. **Nie-rekurencyjnej** (ale jedno-wątkowej),
2. **Równoległej** (wielo-wątkowej), uzyskującej jak najlepszy współczynnik skalowalności.

Dodatkowo, należy załączyć **raport** prezentujący skalowalność wersji równoległej na wybranych testach z różną liczbą wątków.

## Współczynnik skalowalności

Załóżmy, że wersja referencyjna działa na zadanym wejściu w czasie $t_s$, a wersja równoległa uruchomiona na $n$ wątkach pomocniczych w czasie $t_n$. Wtedy współczynnik skalowalności wynosi:
$$\frac{t_s}{t_n}$$

## Wymagania

- Program powinien używać **pthreads**.
- Nie wolno implementować własnych **bitsetów**.
- Kod powinien być kompilowany z **gcc ≥ 12.2** z flagami:
  ```sh
  -std=gnu17 -march=native -O3 -pthread
  ```
- Program będzie ograniczony do **128 MiB** przestrzeni adresowej razy liczba wątków.

## Format wejścia/wyjścia

**Wejście:**
```
t d n m
A_0 (n liczb)
B_0 (m liczb)
```
**Wyjście:**
```
∑A
A (posortowany z krotnościami)
B (posortowany z krotnościami)
```

**Przykład:**
**Wejście:**
```
8 10 0 1
1
```
**Wyjście:**
```
81
9x9
1 8x10
```

## Raport

Raport powinien być w formacie **PDF** i zawierać **wykresy skalowalności** dla:
- $d \in \{5,10,15,20,25,30,32,34\}$,
- $A_0=\emptyset$, $B_0=\{1\}$,
- Liczby wątków: $1, 2, 4, 8, \dots, 64$.
- Współczynnik skalowalności powinien być oznaczony, jeśli czas przekroczy **1 minutę**.

---

Dokument zawiera pełen opis zadania w formacie Markdown do wklejenia do pliku `.md`.

