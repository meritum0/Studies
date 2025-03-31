# Mecze

## Opis Problemu

W treningu piłkarskim bierze udział `n` zawodników (gdzie `n` jest liczbą parzystą). W każdym meczu gracze są podzieleni na dwie drużyny po `n/2` zawodników. 

Trener chce, aby każda para zawodników miała szansę zagrać przeciwko sobie w co najmniej jednym meczu. Jednak w praktyce niektórzy zawodnicy zawsze grają w tych samych drużynach. 

**Kliką** nazywamy największy podzbiór zawodników, którzy we wszystkich meczach grali w tych samych drużynach. Celem jest znalezienie rozmiaru największej kliki.

## Wejście

- Pierwsza linia zawiera dwie liczby całkowite `n` oraz `m` (`4 ≤ n ≤ 40000, 1 ≤ m ≤ 50`), oznaczające liczbę zawodników i liczbę zaplanowanych meczów.
- Kolejne `m` wierszy zawierają `n` parami różnych liczb całkowitych z zakresu `1` do `n`, reprezentujących składy drużyn na poszczególne mecze:
  - Pierwsze `n/2` liczb w każdym wierszu to zawodnicy pierwszej drużyny.
  - Pozostałe `n/2` liczb to zawodnicy drugiej drużyny.

## Wyjście

- Program powinien wypisać jedną liczbę całkowitą – rozmiar największej kliki zawodników.
