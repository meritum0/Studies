# (ENG below) Struktura danych do zapytań o ciąg liczb

## Opis problemu

Twoim zadaniem jest zaimplementowanie struktury danych umożliwiającej efektywne odpowiadanie na zapytania dotyczące ciągu liczb **X**. Ciąg **X** jest dynamicznie rozwijany przez dodawanie nowych elementów. Zapytania mają postać:

**prevInRange(i, [lo, hi])**

Które mają na celu znalezienie największego indeksu `j` (gdzie `0 ≤ j ≤ i`), dla którego `X[j]` znajduje się w zakresie `[lo, hi]`. Jeśli taki indeks nie istnieje, wynik powinien wynosić -1.

Formuła:
```
prevInRange(i, [lo, hi]) = max{0 ≤ j ≤ i: X[j] ∈ [lo, hi]}
```
lub -1 (jeśli taki indeks nie istnieje).

## Funkcje

### 1. **init**
```cpp
void init(const vector<int> &x);
```
Funkcja inicjalizuje początkowy ciąg **X** na podstawie wartości w wektorze `x`. Ciąg może zawierać dowolne wartości mieszczące się w typie `int`.

### 2. **prevInRange**
```cpp
int prevInRange(int i, int lo, int hi);
```
Funkcja oblicza wartość `prevInRange(i, [lo, hi])`. Możesz założyć, że `0 ≤ i < |X|` i `INT_MIN ≤ lo ≤ hi ≤ INT_MAX`.

### 3. **pushBack**
```cpp
void pushBack(int v);
```
Funkcja dodaje element `v` na końcu obecnego ciągu **X**.

### 4. **done**
```cpp
void done();
```
Funkcja zwalnia całą pamięć używaną do obsługiwania ciągu **X**.

## Zasady

- Koszt czasowy funkcji **prevInRange** oraz **pushBack** powinien wynosić **O(log z)** (zamortyzowany), gdzie **z** to liczba różnych wartości w ciągu.  
- Złożoność funkcji **init** powinna wynosić w najgorszym przypadku **O(|X| log z)**, gdzie **z** to zakres typu `int`.
- Unikaj rozwiązań siłowych, które nie spełniają wymaganych czasów.

## Komenda kompilacji

```bash
g++ @opcjeCpp main.cpp prev.cpp -o main.e
```

### Różnice w pliku `opcjeCpp` w stosunku do pliku `opcje` w C

- Zostały usunięte opcje kompilacji:
  - `-Wvla`
  - `-Wjump-misses-init`
  - `-std=c17`
- Została dodana opcja kompilacji:
  - `-std=c++17`

## Uwagi

- Program będzie uruchamiany za pomocą narzędzia `valgrind`, które wykrywa wycieki pamięci. W przypadku wykrycia wycieków pamięci, Twoje rozwiązanie może stracić punkty.
- Użytkownik zwalnia pamięć za pomocą funkcji `free`, dlatego Twoja biblioteka musi alokować pamięć za pomocą funkcji `malloc`.

## Zakończenie

Za poprawne rozwiązanie będziesz punktowany na podstawie efektywności funkcji i braku wycieków pamięci. Upewnij się, że pamięć po obiektach jest prawidłowo zwolniona przez użytkownika.
