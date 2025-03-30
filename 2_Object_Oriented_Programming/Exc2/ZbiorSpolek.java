package gielda;

import java.util.LinkedList;
import java.util.List;
import java.util.Random;

public class ZbiorSpolek {
    private List<Spolka> spolki = new LinkedList<>();

    //*************************************** METODY PUBLIC *******************************************//

    public boolean czyPosiada( Spolka spolka ) {
        for ( Spolka s : spolki ) {
            if ( s.equals(spolka) )
                return true;
        }

        return false;
    }

    public void dodaj( Spolka spolka ) {
        for ( Spolka s : spolki ) {
            if ( s.equals(spolka) ) {
                return;
            }
        }
        spolki.add(spolka);
    }

    // Znajduje spółkę, której akcje powinien kupić inwestor SMA.
    public Spolka znajdzDoKupienia( int tura ) {
        for ( Spolka s : spolki ) {
            if ( s.czyMomentKupna(tura) )
                return s;
        }

        return null;
    }

    // Znajduje spółkę, której akcje powinien sprzedać inwestor SMA.
    public Spolka znajdzDoSprzedazy( Portfel portfel, int tura ) {
        for ( Spolka s : spolki ) {
            if ( portfel.czyPosiada(s) && s.czyMomentSprzedazy(tura) )
                return s;
        }

        return null;
    }

    // Jeśli dana spółka jest już w bazie, to ją zwraca.
    public Spolka podajKonkretnaSpolke( String nazwa ) {
        for ( Spolka s : spolki ) {
            if ( s.nazwa().equals(nazwa) )
                return s;
        }

        return new Spolka(nazwa);
    }

    public Spolka losowaSpolka() {
        if ( spolki.isEmpty() )
            return null;

        Random random = new Random();
        int idx = mod( random.nextInt(), spolki.size() );

        int i = 0;
        for ( Spolka s : spolki ) {
            if ( i++ == idx )
                return s;
        }

        return null;
    }

    // Informuje każdą spółkę o końcu tury - spółki wtedy wykonują odpowiednie operacje.
    public void koniecTury( int tura ) {
        for ( Spolka s : spolki )
            s.koniecTury(tura);
    }

    private int mod( int n , int dzielnik ) {
        if ( dzielnik == 0 )
            throw new ArithmeticException();
        if ( n < 0 )
            return ( n % dzielnik ) + dzielnik - 1;

        return n % dzielnik;
    }
}
