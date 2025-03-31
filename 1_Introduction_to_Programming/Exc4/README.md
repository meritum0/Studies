# (ENG below) Symulacja pracy urzędu

## Opis problemu

Twoim zadaniem jest efektywnie zasymulować zdarzenia występujące w dniu pracy urzędu.  
W urzędzie jest **m okienek**, ponumerowanych od 0 do m-1. W kolejce do każdego okienka ustawiają się interesanci.  
Gdy urzędnik jest wolny, obsługuje pierwszego interesanta z kolejki, po czym ten opuszcza urząd i już tego dnia nie wraca.  
Poza tym w urzędzie zdarzają się różne inne sytuacje, opisane poniżej.

### Funkcje

1. **otwarcie_urzedu**  
   ```cpp
   void otwarcie_urzedu(int m);
   ```
   Funkcja ta zostaje wywołana tylko raz, na początku. Określa liczbę okienek w urzędzie.

2. **nowy_interesant**  
   ```cpp
   interesant* nowy_interesant(int k);
   ```
   Tworzy nowego interesanta i ustawia go w kolejce do okienka o numerze `k`.

3. **numerek**  
   ```cpp
   int numerek(interesant* i);
   ```
   Zwraca numer interesanta `i`. Numerki zaczynają się od 0 i są kolejnymi liczbami całkowitymi.

4. **obsluz**  
   ```cpp
   interesant* obsluz(int k);
   ```
   Obsługuje interesanta w kolejce do okienka `k`. Jeśli kolejka jest pusta, zwraca `NULL`.

5. **zmiana_okienka**  
   ```cpp
   void zmiana_okienka(interesant* i, int k);
   ```
   Przenosi interesanta `i` z kolejki do okienka, do którego nie pasuje, do nowej kolejki okienka `k`.

6. **zamkniecie_okienka**  
   ```cpp
   void zamkniecie_okienka(int k1, int k2);
   ```
   Urzędnik udaje się na przerwę w okienku `k1`. Wszystkie osoby w kolejce do `k1` zostają przeniesione do okienka `k2`.

7. **fast_track**  
   ```cpp
   std::vector<interesant*> fast_track(interesant* i1, interesant* i2);
   ```
   Grupa interesantów (od `i1` do `i2`) przechodzi do okienka specjalnego, gdzie zostaną natychmiast obsłużeni w tej samej kolejności.

8. **naczelnik**  
   ```cpp
   void naczelnik(int k);
   ```
   Naczelnik urzędu, z nudów, odwraca kolejność interesantów w kolejce do okienka `k`.

9. **zamkniecie_urzedu**  
   ```cpp
   std::vector<interesant*> zamkniecie_urzedu();
   ```
   Na koniec dnia urzędnicy obsługują wszystkich pozostałych interesantów we wszystkich kolejkach. Funkcja zwraca wektor interesantów, którzy zostali obsłużeni.

## Zasady

- Funkcja **otwarcie_urzedu** zostanie wywołana jako pierwsza, kiedy rozpoczyna się dzień pracy urzędu.
- **nowy_interesant** dodaje nowego interesanta do konkretnego okienka.
- **obsluz** obsługuje interesanta z danej kolejki, jeśli są dostępni.
- **zmiana_okienka** pozwala interesantowi przenieść się do innego okienka.
- **zamkniecie_okienka** przenosi wszystkich interesantów z jednego okienka do innego.
- **fast_track** pozwala grupie interesantów szybko przejść do okienka specjalnego.
- **naczelnik** odwraca kolejność interesantów w wybranej kolejce.
- Na końcu dnia, funkcja **zamkniecie_urzedu** obsługuje wszystkich pozostałych interesantów i zamyka urząd.

## Szczegóły implementacyjne

- Twoja biblioteka nie zwalnia pamięci po żadnym interesancie. Zwalnianie pamięci po interesantach leży w odpowiedzialności użytkownika biblioteki.
- Pamięć po interesantach powinna być zwalniana za pomocą funkcji `free()`, a nie `delete`.
- Koszt czasowy funkcji każdej funkcji powinien być proporcjonalny do rozmiaru parametrów i wyjścia, z wyjątkiem funkcji `otwarcie_urzedu` i `zamkniecie_urzedu`.

## Komenda kompilacji

```bash
g++ @opcjeCpp main.cpp kol.cpp -o main.e
```

### Różnice w pliku `opcjeCpp` w stosunku do pliku `opcje` w C

- Zostały usunięte opcje kompilacji:
  - `-Wvla`
  - `-Wjump-misses-init`
  - `-std=c17`
- Została dodana opcja kompilacji:
  - `-std=c++17`

## Uwagi

- Program będzie uruchamiany za pomocą narzędzia `valgrind`, które wykrywa wycieki pamięci. W przypadku wykrycia wycieków pamięci Twoje rozwiązanie może stracić punkty.
- Użytkownik zwalnia pamięć za pomocą funkcji `free`, dlatego Twoja biblioteka musi alokować pamięć za pomocą funkcji `malloc`.

## Zakończenie

Za poprawne rozwiązanie będziesz punktowany na podstawie efektywności funkcji i braku wycieków pamięci. Upewnij się, że pamięć po interesantach jest prawidłowo zwolniona przez użytkownika.
