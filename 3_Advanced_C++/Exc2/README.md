# Kolejki napisów

## Wstęp

Biblioteka standardowa języka C++ udostępnia bardzo przydatne kontenery, np. `unordered_map` i `deque`, których nie ma w bibliotece C.

Często potrzebujemy łączyć kod C++ z kodem spadkowym w C. Celem tego zadania jest napisanie w C++ modułu obsługującego kolejki napisów, tak aby można ich używać w C. Moduł składa się z pliku nagłówkowego (z rozszerzeniem `.h`) i pliku z implementacją (z rozszerzeniem `.cpp`).

Rozwiązując to zadanie, studenci powinni poznać:

- kolejne kontenery z STL,
- sposób łączenia kodu w C++ z kodem w C,
- metody inicjowania obiektów globalnych w C++ i wynikające stąd problemy,
- preprocesor, elementy kompilowania warunkowego.

## Polecenie

Moduł `strqueue` powinien udostępniać następujące funkcje:

```cpp
unsigned long strqueue_new();
```
Tworzy nową, pustą kolejkę napisów i zwraca jej identyfikator.

```cpp
void strqueue_delete(unsigned long id);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id`, usuwa ją, a w przeciwnym przypadku nic nie robi.

```cpp
size_t strqueue_size(unsigned long id);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id`, zwraca liczbę jej elementów, a w przeciwnym przypadku zwraca `0`.

```cpp
void strqueue_insert_at(unsigned long id, size_t position, const char* str);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id` oraz `str != NULL`, wstawia napis `str` przed pozycją `position` lub na koniec kolejki, jeżeli wartość `position` jest większa lub równa liczbie napisów w kolejce. Jeżeli kolejka nie istnieje lub `str == NULL`, to nic nie robi. Pozycje w kolejce numerowane są od zera.

```cpp
void strqueue_remove_at(unsigned long id, size_t position);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id` i wartość `position` jest mniejsza niż liczba napisów w kolejce, usuwa napis na pozycji `position`, a w przeciwnym przypadku nic nie robi.

```cpp
const char* strqueue_get_at(unsigned long id, size_t position);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id` i wartość `position` jest mniejsza niż liczba napisów w kolejce, zwraca wskaźnik do napisu na pozycji `position`, a w przeciwnym przypadku zwraca wartość `NULL`.

```cpp
void strqueue_clear(unsigned long id);
```
Jeżeli istnieje kolejka napisów o identyfikatorze `id`, usuwa z niej wszystkie napisy, a w przeciwnym przypadki nic nie robi.

```cpp
int strqueue_comp(unsigned long id1, unsigned long id2);
```
Porównuje leksykograficznie kolejki napisów o identyfikatorach `id1` i `id2`, zwracając:

- `-1`, gdy `kolejka(id1) < kolejka(id2)`,
- `0`, gdy `kolejka(id1) == kolejka(id2)`,
- `1`, gdy `kolejka(id1) > kolejka(id2)`.

Jeżeli kolejka napisów o którymś z identyfikatorów nie istnieje, to jest traktowana jak kolejka pusta.

## Wymagania formalne

- Oczekiwane rozwiązanie powinno korzystać z kontenerów i metod udostępnianych przez standardową bibliotekę C++.
- Nie należy definiować własnych struktur, unii lub klas.
- Kolejka napisów powinna przechowywać kopie napisów, a nie wartości przekazanych jej wskaźników.
- Powinna być też możliwość używania wyżej opisanych funkcji w języku C++.
- Przy kompilowaniu pliku nagłówkowego modułu w C++ deklaracje funkcji powinny znaleźć się w przestrzeni nazw `cxx`.
- Należy ukryć przed światem zewnętrznym wszystkie zmienne globalne i funkcje pomocnicze nienależące do wyspecyfikowanego interfejsu modułu.
- Funkcje powinny wypisywać na standardowe wyjście diagnostyczne informacje o parametrach wywołania i wyniku wykonania.
- Szczegółowy format komunikatów diagnostycznych pokazują zamieszczone przykłady użycia.
- Zachowanie niezmienników i spójność danych można sprawdzać za pomocą asercji.
- Kompilowanie z parametrem `-DNDEBUG` powinno wyłączać wypisywanie informacji diagnostycznych i asercji.
- Obsługa standardowego wyjścia diagnostycznego powinna być realizowana z użyciem strumienia C++, czyli biblioteki `iostream`.
- Nie należy nadużywać kompilowania warunkowego.
- Fragmenty tekstu źródłowego realizujące wyspecyfikowane operacje na kolejkach nie powinny zależeć od sposobu kompilowania (`-DNDEBUG` lub jego brak).

