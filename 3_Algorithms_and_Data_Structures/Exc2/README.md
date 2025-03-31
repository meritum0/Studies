# Matryca

## Opis Problemu

Bajtocki Zakład Poligraficzny (BZP) otrzymał zlecenie na produkcję prążkowanych tapet, które składają się z `n` pionowych pasków o jednakowej szerokości. Niektóre paski mają z góry określone barwy, a reszta może być dowolnie dobrana przez BZP.

Do druku tapet używa się matryc, które drukują pewną liczbę kolejnych pasków. Matryca może być krótsza niż cała tapeta i jest przykładana w `n-k+1` możliwych pozycjach, drukując swoje paski w każdym przyłożeniu. Jeśli dany pasek zostanie zadrukowany różnymi barwami, jego ostateczny kolor będzie ich mieszanką. 

Pracownicy BZP chcą zaprojektować możliwie najkrótszą matrycę, która pozwoli wydrukować całą tapetę. Muszą również zapewnić, że paski określone przez zleceniodawcę są drukowane w dokładnie określonych barwach, bez domieszki innych kolorów.

## Wejście

Program otrzymuje jeden wiersz zawierający napis złożony z wielkich liter alfabetu łacińskiego oraz gwiazdek (`*`), który opisuje wygląd tapety:

- Wielkie litery oznaczają paski o określonych barwach.
- Gwiazdki (`*`) oznaczają paski o dowolnej barwie.
- Długość napisu `n` spełnia warunek: `1 ≤ n ≤ 1 000 000`.

## Wyjście

Program powinien wypisać jedną liczbę całkowitą `k`: minimalną długość matrycy, która pozwala wydrukować całą tapetę zgodnie z wymaganiami.
