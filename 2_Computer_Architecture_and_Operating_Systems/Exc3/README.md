# Pliki z dziurami

Pliki w Linuksie mogą być dziurawe. Na potrzeby tego zadania przyjmujemy, że plik z dziurami składa się z ciągłych fragmentów.  
Na początku fragmentu jest **dwubajtowa długość** w bajtach danych we fragmencie. Potem są dane.  
Fragment kończy się **czterobajtowym przesunięciem**, które mówi, o ile bajtów trzeba się przesunąć od końca tego fragmentu do początku następnego fragmentu.  

- Długość danych w bloku jest **16-bitową liczbą** w naturalnym kodzie binarnym.  
- Przesunięcie jest **32-bitową liczbą** w kodowaniu uzupełnieniowym do dwójki.  
- Liczby w pliku zapisane są w porządku **cienkokońcówkowym** (ang. *little-endian*).  
- Pierwszy fragment zaczyna się na **początku pliku**.  
- Ostatni fragment rozpoznaje się po tym, że jego przesunięcie wskazuje na niego samego.  
- Fragmenty w pliku mogą się **stykać i nakładać**.  

## Suma kontrolna pliku  

Sumę kontrolną pliku obliczamy za pomocą **cyklicznego kodu nadmiarowego** (ang. *CRC*, *cyclic redundancy code*), biorąc pod uwagę dane w kolejnych fragmentach pliku.  
Dane pliku przetwarzamy **bajtami**. Przyjmujemy, że najbardziej znaczący bit bajtu z danymi i wielomianu (dzielnika) CRC zapisujemy po lewej stronie.  

## Polecenie  

Zaimplementuj w asemblerze program `crc`, który oblicza sumę kontrolną danych zawartych w podanym pliku z dziurami:  

```sh
./crc file crc_poly
```

Parametr file to nazwa pliku. Parametr crc_poly to ciąg zer i jedynek opisujący wielomian CRC. Nie zapisujemy współczynika przy najwyższej potędze. Maksymalny stopień wielomianu CRC to 64 (maksymalna długość dzielnika CRC to 65). Przykładowo 11010101 oznacza wielomian x8+x7+x6+x4+x2+1

. Wielomian stały uznajemy za niepoprawny.

Program wypisuje na standardowe wyjście obliczoną sumę kontrolną jako napis składający się z zer i jedynek, zakończony znakiem nowego wiersza \n. Program sygnalizuje poprawne zakończenie kodem 0.

Program powinien korzystać z funkcji systemowych Linuksa: sys_open, sys_read, sys_write, sys_lseek, sys_close, sys_exit.

Program powinien sprawdzać poprawność parametrów i poprawność wykonania funkcji systemowych (z wyjątkiem sys_exit). Jeśli któryś z parametrów jest niepoprawny lub wywołanie funkcji systemowej zakończy się błędem, to program powinien zakończyć się kodem 1. W każdej sytuacji program powinien przed zakończeniem jawnie wywołać funkcję sys_close dla pliku, który otworzył.

Dla uzyskania zadowalającej szybkości działania programu odczyty należy buforować. Należy dobrać optymalny rozmiar bufora, a informację o tym umieścić w komentarzu.