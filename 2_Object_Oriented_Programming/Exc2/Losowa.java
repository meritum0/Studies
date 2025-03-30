package gielda;

import java.util.Random;

public class Losowa extends Strategia {

    //*************************************** METODY PUBLIC *******************************************//

    public void odpytaj( SystemTransakcyjny system, Portfel portfel, Inwestor inwestor ) {
        Random random = new Random();
        int wybor_zlecenia = mod3( random.nextInt() );

        if ( wybor_zlecenia == 0 )
            return;
        if ( wybor_zlecenia == 1 ) {
            Zlecenie z = noweKupno( system, inwestor );
            system.dodajKupno(z);
        }
        else {
            Zlecenie z = nowaSprzedaz( system, portfel, inwestor );
            if ( z != null )
                system.dodajSprzedaz(z);
        }
    }

    public String toString() { return "Random: "; }

    //*************************************** METODY PRIVATE *******************************************//

    private Zlecenie noweKupno( SystemTransakcyjny system, Inwestor inwestor ) {
        Spolka spolka = system.losowaSpolka();
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

    private Zlecenie nowaSprzedaz( SystemTransakcyjny system, Portfel portfel, Inwestor inwestor ) {
        Spolka spolka = portfel.losowaSpolka();
        if ( spolka == null )
            return null;

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
