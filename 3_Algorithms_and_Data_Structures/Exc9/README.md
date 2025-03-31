# Projekty

## Opis Problemu

Bajtazar właśnie awansował na szefa działu informatyki Bardzo Ważnej Instytucji Państwowej. W jego obowiązkach jest zarządzanie projektami informatycznymi. Instytucja przygotowała listę potencjalnych projektów, które powinny zostać wykonane. Niestety wykonanie niektórych projektów zależy od pomyślnego wykonania innych. Dodatkowo, każdy projekt charakteryzuje się minimalną liczbą programistów, którzy są konieczni do jego wykonania.

Ze względu na cięcia budżetowe nie jest możliwe wykonanie wszystkich projektów. Zarząd zdecydował, że zrealizowane zostanie jedynie \(k\) projektów. Bajtazar dostał polecenie zatrudnienia minimalnej liczby programistów, którzy są konieczni do zrealizowania co najmniej \(k\) projektów (przy czym projekty mogą być realizowane sekwencyjnie, tak że programiści są przenoszeni z jednego projektu do drugiego).

Napisz program, który pomoże Bajtazarowi i wyznaczy minimalną liczbę programistów, których należy zatrudnić.

## Wejście

W pierwszym wierszu standardowego wejścia znajdują się trzy liczby całkowite \(n, m, k\) \((1 \leq n \leq 100 000, 0 \leq m \leq 500 000, 0 \leq k \leq n)\), pooddzielane pojedynczymi odstępami i oznaczające odpowiednio liczbę projektów, liczbę zależności pomiędzy projektami oraz minimalną liczbę projektów, które należy zrealizować.

W kolejnych \(n\) wierszach zostały zapisane informacje o liczbie programistów koniecznych do wykonania projektów. W \((i+1)\)-szym wierszu została zapisana liczba całkowita \(p_i\) \((1 \leq p_i \leq 100 000 000)\) oznaczająca, że do wykonania \(i\)-tego projektu konieczne jest zatrudnienie \(p_i\) programistów.

W kolejnych \(m\) wierszach zostały zapisane informacje o zależnościach pomiędzy projektami. Każdy z tych wierszy zawiera dwie liczby całkowite \(a, b\) \((1 \leq a, b \leq n, a \neq b)\) oddzielone pojedynczym odstępem i oznaczające, że do wykonania projektu \(a\) konieczne jest ukończenie projektu \(b\).

Możesz założyć, że zależności pomiędzy projektami nie tworzą cykli.

## Wyjście

W jedynym wierszu standardowego wyjścia należy wypisać minimalną liczbę programistów, których należy zatrudnić, tak by było możliwe wykonanie \(k\) projektów.

