package gielda;

import java.util.ArrayList;
import java.util.List;
import java.util.LinkedList;
import java.util.Random;

public class ZbiorAkcji {
    private List<Akcje> posiadane_akcje = new LinkedList<>();

    //*************************************** METODY PUBLIC *******************************************//

    public boolean czyPosiada( Spolka spolka ) {
        for ( Akcje a : posiadane_akcje ) {
            if ( a.spolka().equals(spolka) )
                return true;
        }

        return false;
    }

    public void dodaj( Akcje akcje ) {
        if ( akcje.liczba() == 0 )
            return;

        for ( Akcje a : posiadane_akcje ) {
            if ( a.equals(akcje) ) {
                a.dodajLiczbe( akcje.liczba() );
                return;
            }
        }
        posiadane_akcje.add(akcje);
    }

    public void odejmij( Akcje akcje ) {
        ArrayList<Akcje> do_usuniecia = new ArrayList<>();
        for ( Akcje a : posiadane_akcje ) {
            if ( a.equals(akcje) ) {
                a.odejmijLiczbe( akcje.liczba() );
                if ( a.liczba() == 0 )
                    do_usuniecia.add(a);
                break;
            }
        }

        for ( Akcje a : do_usuniecia )
            posiadane_akcje.remove(a);
    }

    // Czy posiadamy liczbę akcji danej spółki?
    public boolean czyJeszczeMozliwa( Spolka spolka, int liczba ) {
        for ( Akcje a : posiadane_akcje ) {
            if ( a.spolka().equals(spolka) )
                return a.liczba() >= liczba;
        }
        return false;
    }

    public Spolka losowaSpolka() {
        if ( posiadane_akcje.isEmpty() )
            return null;

        Random random = new Random();
        int idx = mod( random.nextInt(), posiadane_akcje.size() );

        int i = 0;
        for ( Akcje a : posiadane_akcje ) {
            if ( i++ == idx )
                return a.spolka();
        }

        return null;
    }

    // Zwraca liczbę między 1, a liczbą posiadanych akcji danej spółki.
    public int losowaLiczbaDoSprzedazy( Spolka spolka ) {
        for ( Akcje a : posiadane_akcje ) {
            if ( a.spolka().equals(spolka) ) {
                Random random = new Random();
                int ret = mod( random.nextInt(), a.liczba() ) + 1;
                return ret;
            }
        }

        return 0;
    }

    public void wypiszRezultat() {
        for ( Akcje a : posiadane_akcje )
            System.out.print( ", " + a.spolka() + ":" + a.liczba() );

        System.out.print("\n");
    }

    // Metoda potrzebna do testów.
    public int liczbaAkcji( Spolka spolka ) {
        for ( Akcje a : posiadane_akcje ) {
            if ( a.spolka().equals(spolka) )
                return a.liczba();
        }
        return 0;
    }

    //*************************************** METODY PRIVATE *******************************************//

    private int mod( int n , int dzielnik ) {
        if ( dzielnik == 0 )
            throw new ArithmeticException();
        if ( n < 0 )
            return ( n % dzielnik ) + dzielnik - 1;

        return n % dzielnik;
    }
}
