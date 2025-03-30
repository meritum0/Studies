package tramwaje;

import java.util.Scanner;

public class Czytnik {

    public static Symulacja czytaj() {
        Scanner scanner = new Scanner(System.in);
        int dni = Integer.parseInt( scanner.nextLine() );
        int pojemnosc_przystanku = Integer.parseInt( scanner.next() );
        int liczba_przystankow = Integer.parseInt( scanner.next() );

        Przystanek[] przystanki = new Przystanek[liczba_przystankow];

        for ( int i = 0; i < liczba_przystankow; i++ ) {
            String nazwa = scanner.next();
            przystanki[i] = new Przystanek( nazwa, pojemnosc_przystanku );
        }

        int liczba_pasazerow = Integer.parseInt( scanner.next() );

        int pojemnosc_tramwaju = Integer.parseInt( scanner.next() );
        int liczba_linii = Integer.parseInt( scanner.next() );

        Linia[] linie = new Linia[liczba_linii];

        int aktualny_nr_boczny = 0;

        // zbieramy informacje po kolei o liniach
        for ( int i = 0; i < liczba_linii; i++ ) {
            int ile_tramwajow_linii = Integer.parseInt( scanner.next() );
            int dlugosc_trasy = Integer.parseInt( scanner.next() );

            Trasa trasa = new Trasa();

            // zbieramy informacje po kolei o trasie dla danej linii
            for ( int j = 0; j < dlugosc_trasy; j++ ) {
                String nazwa_przystanku = scanner.next();
                int czas_dojazdu = Integer.parseInt( scanner.next() );
                trasa.dodajPrzystanek( Przystanek.wyszukaj( nazwa_przystanku, przystanki ), czas_dojazdu );
            }

            linie[i] = new Linia( i, trasa );
            for ( int j = 0; j < ile_tramwajow_linii; j++ ) {
                linie[i].dodajTramwaj( new Tramwaj(aktualny_nr_boczny, pojemnosc_tramwaju, linie[i]) );
                aktualny_nr_boczny++;
            }
        }

        return new Symulacja( dni, liczba_pasazerow, pojemnosc_przystanku, pojemnosc_tramwaju, przystanki, linie );
    }
}
