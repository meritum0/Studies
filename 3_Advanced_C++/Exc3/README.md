# Turniej rycerski

## Wstęp

Rozwiązując to zadanie, studenci powinni poznać:

- rodzaje i dostępność konstruktorów,
- tworzenie operatorów jako metod klasowych i funkcji globalnych,
- operatory porównujące,
- jawne i niejawne konwersje typów,
- listy inicjujące,
- użycie słów kluczowych `const`, `constexpr`, `consteval` i `constinit`,
- użycie słowa kluczowego `inline`.

## Polecenie

Zadanie polega na zaimplementowaniu modułu `knights` pozwalającego na rozegranie turnieju rycerskiego o puchar króla lub rękę królewny, odciśniętą rzecz jasna w kamieniu. W ramach tego modułu powinny być udostępnione następujące klasy:

- **Knight** – reprezentująca pojedynczego dzielnego rycerza;
- **Tournament** – reprezentująca turniej rycerski.

### Klasa `Knight`

Każdy rycerz klasy `Knight` powinien posiadać pewną liczbę sztuk złota na zacny posiłek po trudach walki i zakup pamiątek w wiosce turniejowej. Co jednak ważniejsze, powinien władać bronią i nosić zbroję, których wartość bojowa jest określona pewną ich klasą. Im wyższa klasa broni albo zbroi, tym są one silniejsze. Klasa o zerowej wartości oznacza brak broni albo zbroi. Złoto oraz klasy broni i zbroi są liczbami całkowitymi typu `size_t`.

#### Wymagania dla klasy `Knight`:

1. **Stała MAX_GOLD** reprezentującą maksymalną liczbę sztuk złota, które może posiadać rycerz (maksymalna wartość typu `size_t`).
2. Tworzenie obiektu (rycerza) z trzema parametrami: liczba sztuk złota, klasa broni i klasa zbroi. Nie powinno być możliwości tworzenia obiektu bez podania tych parametrów.
3. Tworzenie obiektu za pomocą domyślnego kopiowania oraz przenoszenia.
4. Działania przypisania (kopiujące i przenoszące).
5. Pozyskanie informacji o złocie trzymanym przez rycerza, o klasie dzierżonej broni i klasie przywdzianej zbroi.
6. Dorzucenie rycerzowi do sakwy podanej liczby sztuk złota, ale tylko do momentu osiągnięcia dopuszczalnego maksimum.
7. Zmuszenie rycerza do oddania całego złota, zmiany broni na inną o podanej klasie, oddania całej broni, przebrania się w inną zbroję o podanej klasie oraz oddania zbroi.
8. Zabranie drugiemu rycerzowi całego jego złota i tylko lepszych (w sensie klasy) składników jego wyposażenia. Tę operację powinien realizować operator `+=`.
9. Utworzenie nowego rycerza w wyniku operacji „dodawania” (`operator+`) dwóch rycerzy. Nowy rycerz ma mieć tyle złota, ile mają łącznie „dodawani” rycerze, ale maksymalnie MAX_GOLD sztuk, oraz lepsze składniki ich wyposażenia.
10. Porównywanie rycerzy (`operator<=>` i `operator==`), które odpowiada na pytanie, kiedy jeden rycerz może pokonać drugiego. Pojedynek wygrywa przede wszystkim ten wojownik, którego broń jest silniejsza od zbroi przeciwnika i którego klasa zbroi jest jednocześnie nie mniejsza niż klasa broni jego przeciwnika.
11. Wypisywanie na standardowe wyjście informacji o rycerzu w postaci `(liczba sztuk złota, klasa broni, klasa zbroi)`.

Należy zapewnić, żeby obiekt klasy `Knight` mógł być używany w wyrażeniach stałych `constexpr`. Ponadto w wyrażeniach stałych powinny być dostępne wszystkie metody, które nie zmieniają stanu obiektu tej klasy. Oprócz tego powinna być dostępna globalna stała `TRAINEE_KNIGHT` inicjowana tylko podczas kompilowania, która reprezentuje praktykanta rycerskiego nieposiadającego żadnego złota oraz mającego broń i zbroję klasy pierwszej.

### Klasa `Tournament`

W drugiej wymaganej w zadaniu klasie `Tournament` powinna być możliwość:

1. Przechowywania listy pretendentów do tytułu zwycięzcy oraz listy rycerzy wyeliminowanych z turnieju. Kolejność w liście turniejowej określa kolejność, w jakiej rycerze będą stawali do walki. Natomiast rycerze w liście wyeliminowanych są ułożeni w odwrotnej kolejności odpadania z turnieju.
2. Utworzenia obiektu (turnieju) w oparciu o listę rycerzy przekazaną poprzez parametr. Jeśli podana lista będzie pusta, to jedynym pretendentem w turnieju powinien być praktykant rycerski. Tworzenie turnieju bez podania żadnego parametru nie powinno być możliwe.
3. Utworzenia obiektu za pomocą kopiowania i przenoszenia oraz realizacji operacji przypisania w wersji kopiującej i przenoszącej.
4. Dodawania rycerza na koniec listy pretendentów za pomocą operatora `+=`.
5. Wycofania z turnieju wszystkich zawodników mających wszystkie parametry, czyli złoto, klasę broni i klasę zbroi, identyczne jak podany rycerz, wykorzystując w tym celu operator `-=`.
6. Rozegrania turnieju. Wynikiem tej operacji powinien być stały iterator do kontenera z pretendentami, wskazujący zwycięzcę lub koniec kontenera (`end()`) w przypadku braku zwycięzcy turnieju. Zasady turnieju:
    - Do walki stają zawsze dwaj pierwsi pretendenci, którzy zdejmowani są z listy turniejowej;
    - Walka polega na porównaniu tych dwóch pretendentów;
    - Zwycięzca przejmuje złoto przegranego i lepsze składniki jego wyposażenia (broń, zbroję);
    - Wygrany wstawiany jest na koniec listy pretendentów;
    - Przegrany wstawiany jest do listy wyeliminowanych;
    - W przypadku remisu obaj rycerze zachowują swoje złoto i wyposażenie, ale trafiają do listy wyeliminowanych;
    - Turniej kończy się, gdy w liście pretendentów pozostanie jeden (zwycięzca) lub zero (nie ma zwycięzcy) rycerzy;
    - Rycerze trafiający do listy wyeliminowanych wstawiani są na jej początek;
    - Przy remisie kolejność rycerzy wyeliminowanych pozostaje taka sama, jak była na liście pretendentów.
7. Uzyskania stałego iteratora oznaczającego brak zwycięzcy turnieju.
8. Uzyskania informacji o rozmiarze turnieju, czyli łącznej liczbie rycerzy biorących w nim udział (zarówno pretendentów, jak i wyeliminowanych).
9. Wypisania na standardowym wyjściu aktualnej listy pretendentów oraz rycerzy wyeliminowanych. Wydruk każdego pretendenta powinien być poprzedzony prefiksem `+ `, zaś rycerza wyeliminowanego prefiksem `- `. Koniec wydruku należy oznaczać linią zawierającą znak `=`.

Na początku każdego rozegrania turnieju lista wyeliminowanych rycerzy powinna być czyszczona. To samo czyszczenie ma się odbywać także w przypadku każdej operacji zmieniającej zawartość listy pretendentów.

### Funkcja `max_diff_classes`

Na koniec należy zdefiniować globalną funkcję `max_diff_classes`, pobierającą listę rycerzy i dającą w wyniku parę (`std::pair`) z klasami broni i zbroi, których różnica jest w tej liście największa. Funkcja ta może być wykonywana tylko podczas kompilowania.

## Wymagania formalne

Oczekiwane rozwiązanie powinno korzystać z kontenerów i metod udostępnianych przez standardową bibliotekę C++.

Należy ukryć przed światem zewnętrznym wszystkie zmienne globalne i funkcje pomocnicze nienależące do wyspecyfikowanego interfejsu modułu.
