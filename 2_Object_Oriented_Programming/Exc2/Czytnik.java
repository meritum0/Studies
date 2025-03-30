package gielda;

import java.io.File;
import java.util.Scanner;

public class Czytnik {

    //*************************************** METODY PUBLIC *******************************************//

    public static SystemTransakcyjny czytaj( String p ) {
        int ktore_dane = 0;
        boolean czy_nowa_linia;

        SystemTransakcyjny system = new SystemTransakcyjny();

        try {
            File plik = new File(p);
            Scanner scanner = new Scanner(plik);

            while ( scanner.hasNextLine() ) {
                String wiersz = scanner.nextLine();
                if ( wiersz.isEmpty() )
                    continue;

                Scanner slowa = new Scanner(wiersz);
                czy_nowa_linia = true;

                while ( slowa.hasNext() ) {
                    String dana = slowa.next();
                    if ( czy_nowa_linia && dana.charAt(0) == '#' )    // Czy aktualna linia to komentarz?
                        break;

                    if (ktore_dane == 0) {        // Czytamy inwestorów
                        if ( dana.equals("R") )
                            system.nowyRandom();
                        else if ( dana.equals("S") )
                            system.nowySMA();
                        else if (czy_nowa_linia && czyAkcje(dana)) {
                            ktore_dane++;   // Czytamy już akcje
                            system.dodajSpolke( stringNaSpolke(system, dana) );
                        }
                        else
                            throw new ZleDaneWPliku();
                    }
                    else if ( ktore_dane == 1 ) {   // Czytamy akcje w obiegu i ich ceny
                        if (  czyAkcje(dana) )
                            system.dodajSpolke( stringNaSpolke(system, dana) );
                        else if ( czy_nowa_linia ) {
                            int liczba = Integer.parseInt(dana); // Wyrzuci wyjątek, jeśli dana to nie liczba
                            ktore_dane++;
                            system.ustawGotowke(liczba);
                        }
                        else
                            throw new ZleDaneWPliku();
                    }
                    else if ( ktore_dane == 2 ) {
                        if ( czyAkcje(dana) && czyWSystemie(stringNaAkcje(system, dana), system) ) {
                            system.dodajStartoweAkcje( stringNaAkcje(system, dana) );
                        }
                        else
                            throw new ZleDaneWPliku();
                    }

                    czy_nowa_linia = false;
                }
                slowa.close();
            }
            scanner.close();
            return system;
        }
        catch ( Exception e ) {
            System.out.println( "Wystąpił błąd podczas (próby) czytania z pliku." );
            return null;
        }
    }

    //*************************************** METODY PRIVATE *******************************************//


    // Zwraca, czy dany String opisuje akcje pewnej spółki
    private static boolean czyAkcje( String dana ) {
        if ( dana.isEmpty() )
            return false;
        int i = 0;
        for (; i < 5; i++ ) {
            if ( i > 0 && dana.charAt(i) == ':')
                break;
            if ( !Character.isLetter(dana.charAt(i)) || !Character.isUpperCase(dana.charAt(i)) )
                return false;
        }
        if ( dana.charAt(i++) != ':' )
            return false;

        if ( dana.length() <= i)
            return false;

        while ( i < dana.length() ) {
            if ( !Character.isDigit(dana.charAt(i)) )
                return false;
            i++;
        }
        return true;
    }

    private static Spolka stringNaSpolke( SystemTransakcyjny system, String dane ) {
        String spolka = "";
        String cena = "";
        int i = 0;
        for ( ; dane.charAt(i) != ':'; i++ ) {
            spolka += dane.charAt(i);
        }

        i++;

        for ( ; i < dane.length(); i++ ) {
            cena += dane.charAt(i);
        }

        Spolka sp = system.podajKonkretnaSpolke(spolka);
        sp.ostatniaTransakcja( Integer.parseInt(cena) );

        return sp;
    }

    private static Akcje stringNaAkcje( SystemTransakcyjny system, String dane ) {
        String spolka = "";
        String liczba = "";
        int i = 0;
        for ( ; dane.charAt(i) != ':'; i++ ) {
            spolka += dane.charAt(i);
        }

        i++;

        for ( ; i < dane.length(); i++ ) {
            liczba += dane.charAt(i);
        }

        Spolka sp = system.podajKonkretnaSpolke(spolka);

        return new Akcje( sp, Integer.parseInt(liczba) );
    }

    private static boolean czyWSystemie( Akcje akcje, SystemTransakcyjny system ) {
        return system.czyPosiada( akcje.spolka() );
    }
}
