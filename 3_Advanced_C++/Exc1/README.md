# Klasyfikacja medalowa

## Wstęp

Biblioteka standardowa języka C++ udostępnia implementacje wielu struktur danych, takich jak `pair`, `tuple`, `array`, `vector`, `unordered_set`, `set`, `string`, `unordered_map`, `map`, `queue` itp., a także implementacje podstawowych algorytmów, np. `sort`, `lower_bound`, `upper_bound`, `max_element` itd. Celem pierwszego zadania zaliczeniowego jest przećwiczenie korzystania z tej biblioteki. Studenci powinni:

- poznać podstawy korzystania z STL-a,
- uświadomić sobie konieczność weryfikacji poprawności danych wejściowych,
- nauczyć się podejmowania decyzji programistycznych,
- ugruntować w sobie konieczność testowania programu.

## Polecenie

Napisać program wyznaczający klasyfikację medalową krajów uczestniczących w igrzyskach olimpijskich. Program czyta dane ze standardowego wejścia. Poprawna linia z danymi wejściowymi ma jeden z następujących trzech formatów:

1. Informacja o zdobyciu przez kraj medalu.
2. Informacja o odebraniu krajowi medalu.
3. Prośba o wypisanie aktualnej klasyfikacji medalowej.

### Format danych wejściowych

- **Zdobycie medalu:**
  - Składa się z nazwy kraju i rodzaju medalu oddzielonych pojedynczą spacją.
  - Nazwa kraju zawiera tylko litery języka angielskiego, składa się z co najmniej dwóch liter, pierwsza litera jest wielka, wewnątrz nazwy mogą występować spacje.
  - Rodzaj medalu to cyfra `1` (złoto), `2` (srebro) lub `3` (brąz). Cyfra `0` oznacza tylko dodanie kraju do rankingu bez medalu.
  - Kraj może być dodawany do rankingu wielokrotnie.

- **Odebranie medalu:**
  - Linia zaczyna się od znaku `-`, po którym następuje informacja o medalu w takim samym formacie jak przy zdobyciu.
  - Rodzaj odbieranego medalu jest cyfrą dodatnią.

- **Wyświetlenie klasyfikacji:**
  - Linia zaczyna się od znaku `=`.
  - Po nim występują trzy liczby całkowite z przedziału `1-999999`, oddzielone pojedynczą spacją.
  - Liczby określają wagi medali w kolejności: złote, srebrne, brązowe.
  - Jeśli nie ma jeszcze medali, nic się nie wypisuje.

### Format danych wyjściowych

- Program wypisuje klasyfikację medalową na standardowe wyjście.
- Każdy kraj wypisuje się w osobnej linii, poprzedzając go miejscem w klasyfikacji.
- W przypadku remisu kraje wypisuje się w kolejności leksykograficznej z tym samym miejscem.
- Każda linia kończy się znakiem nowej linii.

## Obsługa błędów

Program na bieżąco sprawdza poprawność danych wejściowych. Za błędną uznaje się także informację o odebraniu medalu, którego kraj nie posiada. W przypadku błędnej linii program wypisuje na standardowe wyjście diagnostyczne komunikat:

```
ERROR L
```

gdzie `L` oznacza numer linii (liczone od `1`). Program ignoruje zawartość błędnych linii.

## Wymagania formalne

- Program powinien zakończyć się kodem `0`.
- Rozwiązanie nie powinno zawierać definicji własnych struktur i klas przechowujących dane.
- Należy intensywnie korzystać z kontenerów i algorytmów STL.
- Obsługa wejścia i wyjścia powinna być realizowana za pomocą strumieni.

## Kompilacja

Rozwiązanie należy umieścić w pliku `medals.cpp` i przesłać do Moodle. Program będzie kompilowany na maszynie `students` poleceniem:

```sh
g++ -Wall -Wextra -O2 -std=c++20 medals.cpp -o medals
```

