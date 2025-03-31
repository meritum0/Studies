# Trzy motele

Wzdłuż autostrady, którą możemy sobie wyobrazić jako linię prostą, rozmieszczonych jest `n` moteli. Motele numerujemy od `1` do `n` kolejno wzdłuż autostrady. Każdy z moteli należy do jakiejś sieci moteli, którą opisujemy liczbą z zakresu od `1` do `n`.

## Problem

Bajtek powiedział Bitkowi, że w trakcie jazdy autostradą **trzy razy zatrzymał się na noc w motelu**, przy czym za każdym razem nocował w motelu innej sieci.  
Bitek zastanawia się, **w jak dużych odstępach** musiały znajdować się te trzy motele. Interesuje go:

- **Najbliższa trójka moteli**  
- **Najdalsza trójka moteli**

## Definicja formalna

Bitek chciałby wyznaczyć **trzy motele** `A`, `B`, `C`, położone w tej kolejności wzdłuż autostrady i należące do różnych sieci, dla których:

- **Najbliższa trójka** – maksimum z odległości od `A` do `B` i od `B` do `C` jest jak najmniejsze.
- **Najdalsza trójka** – minimum z odległości od `A` do `B` i od `B` do `C` jest jak największe.

## Wejście

Program wczytuje z `stdin`:

1. Liczbę `n` – liczbę moteli.
2. `n` wierszy, w których każdy wiersz zawiera dwie liczby całkowite:
   - `s_i` – numer sieci, do której należy dany motel.
   - `d_i` – odległość motelu od początku autostrady.

## Wyjście

Program wypisuje na `stdout` dwie liczby całkowite:

1. **Maksimum z minimów odległości** trzech moteli należących do różnych sieci.
2. **Minimum z maksimów odległości** trzech moteli należących do różnych sieci.

Jeśli **nie istnieje trójka moteli należących do różnych sieci**, program powinien wypisać `0 0`.

## Ograniczenia

- `1 ≤ n ≤ 1 000 000`
- `s_i` mieści się w zakresie `1` do `n`
- `d_i` są liczbami nieujemnymi mieszczącymi się w `int`
- Kilka moteli może znajdować się w tym samym punkcie autostrady.
- Możliwe, że Bajtek nocował w dwóch lub trzech motelach w tym samym miejscu.

