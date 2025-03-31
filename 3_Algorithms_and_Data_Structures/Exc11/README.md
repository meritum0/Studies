# Ogród Bajtocji

Rząd Bajtocji postanowił przeznaczyć sporą kwotę na stworzenie wspaniałego ogrodu. Prace nad ogrodem nadzoruje profesor Makary. Profesor zdecydował się na ogród składający się z jednej bardzo długiej alei, wzdłuż której rosnąć będą drzewa. Aleja została podzielona na `n` segmentów, ponumerowanych 1, 2, ..., n. Początkowo drzewa we wszystkich segmentach mają wysokość 1. Każdego dnia profesor Makary nawozi pewną liczbę sąsiadujących ze sobą segmentów, każdy tą samą ilością specjalnego, opracowanego przez siebie nawozu. W efekcie drzewa w tych segmentach natychmiast rosną.

Jak wykazały wcześniejsze badania profesora Makarego, największe wrażenie na odwiedzających robią drzewa ustawione w porządku niemalejących wysokości. Dlatego profesor czasem przygląda się różnym częściom ogrodu i oblicza dla nich długość najdłuższego spójnego ciągu segmentów, w którym wysokości drzew układają się w ciąg niemalejący. Napisz program, który pomoże mu w tym zadaniu.

## Wejście

W pierwszym wierszu wejścia podane są dwie liczby całkowite: `n`, `m` (1 ≤ n ≤ 500000, 1 ≤ m ≤ 500000), które są odpowiednio liczbą segmentów, oraz liczbą akcji wykonanych przez profesora.

W każdym z kolejnych m wierszy znajduje się opis jednej akcji. Jeśli i-ty wiersz zaczyna się od litery 'N', to opisuje akcję nawożenia. Ma on wtedy postać `N ai bi ki`, gdzie 1 ≤ ai ≤ bi ≤ n oraz 1 ≤ ki ≤ 1000 są liczbami całkowitymi. Oznaczają one, że profesor nawiózł segmenty od `ai` do `bi` ilością nawozu, która powoduje wzrost drzew w tych segmentach o `ki`.

Jeśli natomiast i-ty wiersz zaczyna się od litery 'C', to opisuje on akcję szukania ciągu drzew ustawionych w porządku niemalejących wysokości. Taki wiersz ma postać `C ai bi`, gdzie 1 ≤ ai ≤ bi ≤ n są liczbami całkowitymi. Oznaczają one, że profesor chce obliczyć długość najdłuższego spójnego ciągu segmentów o niemalejących wysokościach drzew, wśród segmentów od `ai` do `bi`.

## Wyjście

Twój program powinien wypisać dla każdej akcji 'C' wiersz zawierający jedną liczbę całkowitą. Liczba ta ma być długością odpowiedniego ciągu segmentów, szukanego przez profesora. Możesz założyć, że zostanie wykonana przynajmniej jedna akcja 'C'.

---



