package gielda;

import java.util.Random;

public class SMA extends Strategia {

    //*************************************** METODY PUBLIC *******************************************//

    public void odpytaj( SystemTransakcyjny system, Portfel portfel, Inwestor inwestor ) {
        if ( system.tura() < 10 )
            return;

        Spolka spolka = system.znajdzDoKupienia();
        if ( spolka != null ) {
            Zlecenie zlecenie = noweKupno( system, spolka, inwestor );
            system.dodajKupno(zlecenie);
            return;
        }

        spolka = system.znajdzDoSprzedazy(portfel);
        if ( spolka != null ) {
            Zlecenie zlecenie = nowaSprzedaz( system, portfel, spolka, inwestor );
            system.dodajSprzedaz(zlecenie);
        }
    }

    public String toString() { return "SMA:    "; }

    //*************************************** METODY PRIVATE *******************************************//

    private Zlecenie noweKupno( SystemTransakcyjny system, Spolka spolka, Inwestor inwestor ) {
        int liczba = spolka.losowaLiczbaDoKupienia();
        int cena = spolka.losowaCena();
        String id = system.podajId();

        boolean kupno = true;

        Random random = new Random();
        int wybor_typu = mod3( random.nextInt() );

        if ( wybor_typu == 0 )
            return new Natychmiastowe( kupno, id, inwestor, spolka, liczba, cena );
        else if ( wybor_typu == 1)
            return new BezTerminu( kupno, id, inwestor, spolka, liczba, cena );
        else
            return new DoTury( kupno, id, inwestor, spolka, liczba, cena, system.losowaTura(), system );
    }

    private Zlecenie nowaSprzedaz( SystemTransakcyjny system, Portfel portfel, Spolka spolka, Inwestor inwestor ) {
        int liczba = portfel.losowaLiczbaDoSprzedazy(spolka);
        int cena = spolka.losowaCena();
        String id = system.podajId();

        boolean sprzedaz = false;

        Random random = new Random();
        int wybor_typu = mod3( random.nextInt() );

        if ( wybor_typu == 0 )
            return new Natychmiastowe( sprzedaz, id, inwestor, spolka, liczba, cena );
        else if ( wybor_typu == 1)
            return new BezTerminu( sprzedaz, id, inwestor, spolka, liczba, cena );
        else
            return new DoTury( sprzedaz, id, inwestor, spolka, liczba, cena, system.losowaTura(), system );
    }

    private int mod3( int n ) {
        if ( n < 0 )
            return ( n % 3 ) + 2;

        return n % 3;
    }
}
