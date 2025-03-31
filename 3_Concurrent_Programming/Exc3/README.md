# Asynchroniczny Egzekutor w C

## Motywacja

Współczesne systemy komputerowe często muszą radzić sobie z wieloma zadaniami jednocześnie: obsługą licznych połączeń sieciowych, przetwarzaniem dużych ilości danych czy komunikacją z urządzeniami zewnętrznymi. Tradycyjne podejście oparte na wielowątkowości może prowadzić do znacznego narzutu na zarządzanie wątkami, a także do problemów ze skalowalnością w systemach o ograniczonych zasobach.

Asynchroniczne egzekutory (*asynchronous executors*) stanowią nowoczesne rozwiązanie tego problemu. Zamiast uruchamiać każde zadanie w oddzielnym wątku, egzekutor zarządza zadaniami w sposób kooperacyjny – zadania "ustępują" sobie nawzajem procesora w momentach, gdy czekają na zasoby, np. dane z pliku, sygnały z urządzenia czy odpowiedź z bazy danych. Dzięki temu można obsłużyć tysiące lub nawet miliony jednoczesnych operacji przy użyciu ograniczonej liczby wątków.

## Przykłady Zastosowań

- **Serwery HTTP** – serwery takie jak Nginx czy Tokio w Rust wykorzystują asynchroniczne egzekutory do obsługi tysięcy połączeń sieciowych, jednocześnie minimalizując zużycie pamięci i procesora.
- **Programowanie systemów wbudowanych** – w systemach o ograniczonych zasobach (np. mikrokontrolerach) efektywne zarządzanie wieloma zadaniami, takimi jak odczytywanie danych z czujników czy sterowanie urządzeniami, jest kluczowe. Asynchroniczność pozwala unikać kosztownej wielowątkowości.
- **Aplikacje interaktywne** – w aplikacjach graficznych lub mobilnych asynchroniczność zapewnia płynność działania – np. podczas pobierania danych z sieci interfejs użytkownika pozostaje responsywny.
- **Przetwarzanie danych** – asynchroniczne podejście pozwala efektywnie zarządzać wieloma równoczesnymi operacjami wejścia-wyjścia, np. w systemach analizy danych, gdzie operacje dyskowe mogą stanowić wąskie gardło.

## Cel zadania

W ramach tego zadania domowego napiszecie uproszczoną wersję jednowątkowego asynchronicznego egzekutora w języku C (inspirowaną biblioteką Tokio w języku Rust). W tym zadaniu domowym nie tworzymy nigdzie wątków: egzekutor i wykonywane przez niego zadania wszystkie działają w wątku głównym (lub ustępują innym czekając na np. możliwość czytania).

Otrzymacie obszerny szkielet projektu i będziecie mieli za zadanie zaimplementować kluczowe elementy. W efekcie powstanie prosta, ale funkcjonalna biblioteka asynchroniczna. Przykłady jej zastosowania wraz z opisami znajdują się w plikach źródłowych w `tests/` (które stanowią też podstawowe, przykładowe testy).

## Specyfikacja

### Struktury

Najważniejsze struktury potrzebne w implementacji to:

- **Executor**: odpowiedzialny za wykonywanie zadań na procesorze – implementuje główną pętlę zdarzeń `executor_run()` i zawiera kolejkę zadań.
- **Mio**: odpowiedzialny za komunikację z systemem operacyjnym – implementuje czekanie na np. dostępność danych do odczytu, tzn. woła funkcje `epoll_*`.
- **Future**: zawiera korutynę (coroutine), czyli informacje o tym, jakie zadanie ma być wykonane oraz stan wymagany między kolejnymi etapami.
- **Waker**: callback (wskaźnik na funkcję) definiujący, jak powiadomić egzekutora o tym, że zadanie może postępować dalej.

## Schemat działania

### Stany Future:

1. **COMPLETED**: zadanie się zakończyło i ma wynik.
2. **FAILURE**: zadanie się zakończyło błędem lub zostało przerwane.
3. **PENDING (zakolejkowany)**: czeka na przydział procesora.
4. **PENDING (waker czeka)**: zadanie nie mogło dalej postępować i ktoś trzyma `Waker`, który wywoła je ponownie.

### Egzekutor w pętli przetwarza zadania:

1. Jeśli nie ma niezakończonych zadań, kończy pętlę.
2. Jeśli nie ma aktywnych zadań, woła `mio_poll()`, by uśpić wątek.
3. Dla każdego aktywnego zadania woła `future.progress(future, waker)`.

## Future do zaimplementowania: kombinatory

- **ThenFuture**: wykonuje `fut1`, a następnie `fut2`, przekazując wynik `fut1` do `fut2`.
- **JoinFuture**: wykonuje `fut1` i `fut2` współbieżnie, kończąc gdy oba zakończą działanie.
- **SelectFuture**: wykonuje `fut1` i `fut2` współbieżnie, kończąc gdy którekolwiek zakończy działanie.

Szczegółowy interfejs i zachowanie są opisane w `future_combinators.h`.

