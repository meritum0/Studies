# Liczniki

## Wstęp

W standardzie C++20 wprowadzono moduły, które są nowym rozszerzeniem języka C++. Chociaż ich implementacja w dostępnych kompilatorach nie jest jeszcze kompletna, a w niektórych przypadkach eksperymentalna, warto zapoznać się z tym nowym elementem języka. Celem tego zadania jest:
- zapoznanie się z podstawami używania modułów w C++,
- zrozumienie paradygmatu programowania obiektowego w C++.

## Definicja Licznika i Rodzaje Liczników

Licznik jest układem cyfrowym, który zlicza impulsy pojawiające się na jego wejściu. Licznik ma dzielnik wstępny o wartości `<P>`. Po każdym zliczonym impulsie licznik pomija kolejnych `<P>` impulsów. Licznik zgłasza zdarzenia po osiągnięciu określonych wartości. Wartości licznika oraz wartość parametru `<P>` są liczbami całkowitymi w przedziale od 0 do `UINT64_MAX`.

Mamy kilka typów liczników:

1. **Licznik Modulo**:
   - Zlicza od zera do wartości maksymalnej `<M>`, po czym wraca do zera.
   - Zgłasza zdarzenie przepełnienia, gdy jego wartość zmienia się z `<M>` na zero.
   - Jeśli wartość maksymalna jest równa zero, licznik zawsze ma wartość zero i zgłasza przepełnienie przy każdym zliczonym impulsie.

2. **Licznik Fibonacciego**:
   - Zlicza od zera do `UINT64_MAX`, po czym się zatrzymuje.
   - Zgłasza zdarzenie, gdy jego wartość osiąga liczbę Fibonacciego (1, 2, 3, 5, 8, itd.).

3. **Licznik Geometryczno-Dziesiętny**:
   - Zlicza w kolejnych cyklach od 0 do 9, od 0 do 99, od 0 do 999, …, od 0 do 999999999999, po czym wraca do 0.
   - Zgłasza zdarzenie przepełnienia, gdy jego wartość zmienia się z maksymalnej na zero.

## Polecenie

Należy napisać program obsługujący zbiór liczników. Program czyta dane ze standardowego wejścia i obsługuje różne komendy. Poprawna linia z danymi wejściowymi może mieć jeden z następujących formatów:

1. **M** `<C> <P> <M>`:  
   Tworzy licznik modulo o numerze `<C>` z parametrami `<P>`, `<M>`.

2. **F** `<C> <P>`:  
   Tworzy licznik Fibonacciego o numerze `<C>` z parametrem `<P>`.

3. **G** `<C> <P>`:  
   Tworzy licznik geometryczno-dziesiętny o numerze `<C>` z parametrem `<P>`.

4. **D** `<C>`:  
   Usuwa licznik o numerze `<C>`.

5. **P** `<C>`:  
   Wypisuje literę `C`, spację, numer licznika, spację, wartość licznika i znak nowej linii.

6. **A** `<T>`:  

   Wysyła do wszystkich liczników `<T>` impulsów. Powoduje wypisanie na standardowe wyjście zdarzeń wygenerowanych przez wszystkie liczniki od poprzedniego wykonania tego polecenia lub od początku działania programu, jeśli to polecenie jest wykonywane pierwszy raz. Każde zdarzenie wypisywane jest w osobnej linii zakończonej znakiem nowej linii i składa się z litery E, spacji, numeru licznika `<C>` zgłaszającego to zdarzenie, spacji i liczby impulsów od poprzedniego wykonania tego polecenia lub od początku działania programu, jeśli to polecenie jest wykonywane pierwszy raz. Zdarzenia wypisywane są chronologiczne według ich czasu zgłoszenia. Zdarzenia zgłoszone w tym samym czasie są sortowane rosnąco w kolejności numerów liczników je zgłaszających. Jeśli nie ma zdarzeń, to niczego nie wypisuje.

Początkowa wartość nowego licznika wynosi zero. Parametry `<C>`, `<P>`, `<M>`, `<T>` są liczbami całkowitymi z przedziału od 0 do UINT64_MAX.

Nazwa polecenia i jego parametry oddzielone są pojedynczą spacją. Linie wejściowe są rozdzielone znakiem nowej linii. Po ostatniej linii znak nowej linii jest opcjonalny. Poprawne dane wejściowe nie zawierają innych białych znaków niż wymienione powyżej.

### Obsługa Błędów

Program sprawdza błędne dane wejściowe. Błędy obejmują:
- próbę utworzenia nowego licznika, jeśli licznik o podanym numerze już istnieje,
- próbę usunięcia nieistniejącego licznika.

W przypadku błędu program wypisuje na standardowe wyjście komunikat:

ERROR L

gdzie `L` oznacza numer linii, w której wystąpił błąd.

## Wymagania Formalne

- Program kończy się kodem 0.
- Implementacja musi być podzielona na moduły. Każdy moduł powinien znajdować się w osobnym pliku z rozszerzeniem `.cppm`.
- Program powinien wykorzystywać paradygmat programowania obiektowego.

## Implementacja

Program powinien implementować klasy dla różnych typów liczników, takich jak licznik modulo, licznik Fibonacciego, licznik geometryczno-dziesiętny. Wykorzystanie modułów umożliwi utrzymanie kodu w czystości i podzielności.

Nie wolno korzystać z poleceń preprocesora zaczynających się znakiem `#`.

