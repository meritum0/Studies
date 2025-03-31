# Malowanie Autostrady

## Opis Problemu

Profesor Makary maluje autostradę o długości `n` kilometrów białą farbą. Autostrada jest podzielona na kilometrowe odcinki ponumerowane od `1` do `n` i początkowo cała jest czarna.

Działania profesora:
- W nocy, jeśli cierpi na bezsenność, maluje pewien odcinek autostrady (oznaczone literą **B**).
- W dzień, gdy pojawiają się dziury, przyjeżdża walec i kładzie asfalt, zamieniając dany odcinek z powrotem na czarny (oznaczone literą **C**).

Po każdej operacji profesor chce znać liczbę kilometrów autostrady pomalowanych na biało.

## Wejście

- Pierwszy wiersz: jedna liczba całkowita `n` (`1 ≤ n ≤ 1 000 000`) – długość autostrady.
- Drugi wiersz: jedna liczba całkowita `m` (`1 ≤ m ≤ 1 000 000`) – liczba operacji malowania i asfaltowania.
- Następne `m` wierszy: każda linia zawiera:
  - Dwie liczby całkowite `a, b` (`1 ≤ a ≤ b ≤ n`) – przedział operacji.
  - Jedną literę `c` (`B` – malowanie, `C` – asfaltowanie).

## Wyjście

- Po każdej operacji program powinien wypisać liczbę kilometrów pomalowanych na biało.
