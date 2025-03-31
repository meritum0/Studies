# Skoroszyt

## Wstęp

Studenci powinni poznać:

- poziomy odporności na wyjątki;
- schematy pozwalające zapewnić co najmniej silną odporność na wyjątki;
- zarządzanie pamięcią z użyciem sprytnych wskaźników;
- schemat implementowania semantyki kopiowania przy modyfikowaniu.

## Polecenie

W celu ułatwienia sobie nauki do egzaminów grupa studentów informatyki postanowiła usystematyzować swój sposób gromadzenia notatek za pomocą skoroszytu (ang. binder). Aby ta metoda była praktyczna i rzeczywiście usystematyzowana, studenci uznali, że skoroszyty, których będą używać, muszą spełniać pewne podstawowe wymagania.

Format treści notatek nie powinien być z góry narzucony (w końcu jedni studenci ręcznie notują na zajęciach, podczas gdy inni wolą wykorzystywać w tym celu sprzęt elektroniczny), jednak spójny w obrębie pojedynczego skoroszytu. Dodatkowo każda notatka powinna być widocznie oznaczona za pomocą (unikalnej w obrębie pojedynczego skoroszytu) zakładki, aby ułatwić jej wyszukiwanie.

Należy zaimplementować szablon klasy `binder` dostępny w przestrzeni nazw `cxx` o następującej deklaracji:

```cpp
namespace cxx {
  template <typename K, typename V> class binder;
}
```

gdzie typ klucza (zakładki) `K` ma semantykę wartości, co oznacza, że dostępne są dla niego:
- bezparametrowy konstruktor domyślny,
- konstruktor kopiujący,
- konstruktor przenoszący,
- operatory przypisania,
- destruktor.

Na typie `K` zdefiniowany jest porządek liniowy, a obiekty tego typu mogą być porównywane. O typie `V` (treści notatki) założono, że posiada publiczny konstruktor kopiujący oraz publiczny destruktor.

Studenci chętnie dzielą się swoimi notatkami. Z racji oszczędności preferują współdzielenie tego samego skoroszytu zamiast tworzenia jego fizycznych kopii. Jednak w sytuacji, gdy jeden z użytkowników skoroszytu wprowadza zmiany, aby nie utrudniać nauki innym, konieczne staje się stworzenie osobnej kopii skoroszytu.

### Semantyka Kopiowania przy Modyfikowaniu (Copy on Write)

Kopiowanie przy modyfikowaniu to technika optymalizacji, polegająca na tym, że po stworzeniu kopii obiektu (np. przez konstruktor kopiujący lub operator przypisania), obie kopie współdzielą te same zasoby (przechowywane na stercie) do momentu, w którym jedna z kopii będzie wymagała modyfikacji. Wówczas tworzy ona swoją własną kopię zasobów, aby móc przeprowadzić modyfikację.

## Operacje na klasie `binder`

Aby umożliwić efektywne zbieranie i modyfikowanie notatek, klasa `binder` powinna udostępniać poniższe operacje. Złożoność czasowa operacji zakłada, że nie trzeba wykonywać kopii:

### Konstruktory
- **binder()**: Bezparametrowy konstruktor tworzący pusty skoroszyt. Złożoność: O(1).
- **binder(const binder &)**: Konstruktor kopiujący. Złożoność: O(1).
- **binder(binder &&)**: Konstruktor przenoszący. Złożoność: O(1).

### Operator przypisania
- **operator=(binder)**: Operator przypisania przyjmujący argument przez wartość. Złożoność: O(1), plus czas niszczenia nadpisywanego obiektu.

### Metody modyfikujące

- **void insert_front(K const &k, V const &v)**: Wstawia notatkę z zakładką na początek skoroszytu. Jeśli zakładka jest już używana, zgłasza wyjątek `std::invalid_argument`. Złożoność: O(log n).
- **void insert_after(K const &prev_k, K const &k, V const &v)**: Wstawia notatkę z zakładką `k` bezpośrednio po notatce z zakładką `prev_k`. W przypadku braku `prev_k` w skoroszycie, zgłasza wyjątek `std::invalid_argument`. Złożoność: O(log n).
- **void remove()**: Usuwa pierwszą notatkę ze skoroszytu. Jeśli skoroszyt jest pusty, zgłasza wyjątek `std::invalid_argument`. Złożoność: O(log n).
- **void remove(K const &)**: Usuwa notatkę z podaną zakładką. Jeśli notatka nie istnieje, zgłasza wyjątek `std::invalid_argument`. Złożoność: O(log n).

### Metody odczytu

- **V &read(K const &)**: Zwraca referencję do notatki o podanej zakładce, umożliwiając jej modyfikację. W przypadku, gdy notatka nie istnieje, zgłasza wyjątek `std::invalid_argument`. Złożoność: O(log n).
- **V const &read(K const &) const**: Zwraca referencję do notatki o podanej zakładce w wersji stałej. Złożoność: O(log n).

### Metody pomocnicze

- **size_t size() const**: Zwraca liczbę notatek w skoroszycie. Złożoność: O(1).
- **void clear()**: Usuwa wszystkie notatki ze skoroszytu. Złożoność: O(n).

### Iterator

Skoroszyt powinien umożliwiać szybkie przeczytanie wszystkich notatek poprzez iterator:
- **Iterator const_iterator**: Metody `cbegin`, `cend` oraz operatory przypisania (`=`), porównania (`==`, `!=`), inkrementacji (prefiksowy i postfiksowy operator `++`), dostępu do wartości (`*`, `->`).
- Wszystkie operacje wykonują się w czasie O(1), pod warunkiem, że nie zachodzi kopiowanie skoroszytu.
- Iterator działa na zasadzie `const_iterator`, pozwalając na przeglądanie notatek, ale nie na ich modyfikację.

## Zasady Projektowania

- Klasa `binder` powinna zapewniać silną gwarancję odporności na wyjątki.
- Tam, gdzie to możliwe i pożądane, metody powinny być oznaczone kwalifikatorami `const` oraz `noexcept`.
- Obiekt klasy `binder` powinien przechowywać tylko jedną kopię każdej zakładki i notatki.
- W przypadku modyfikacji skoroszytu, operacje modyfikujące zakończone niepowodzeniem nie mogą unieważniać iteratorów.

## Kompilacja

Rozwiązanie będzie kompilowane za pomocą polecenia:

```bash
g++ -Wall -Wextra -O2 -std=c++20 *.cpp
