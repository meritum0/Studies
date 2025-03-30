package gielda;

import java.util.LinkedList;
import java.util.List;

public class ZbiorInwestorow {
    private List<Inwestor> inwestorzy = new LinkedList<>();

    //*************************************** METODY PUBLIC *******************************************//

    public void ustawGotowke( int liczba ) {
        for ( Inwestor i : inwestorzy )
            i.dodajGotowke(liczba);
    }

    public void dodajStartoweAkcje( Akcje akcje ) {
        for ( Inwestor i : inwestorzy ) {
            Akcje a = new Akcje( akcje.spolka(), akcje.liczba() );
            i.dodajAkcje( a, 0 );
        }
    }

    public void nowyRandom() {
        inwestorzy.add( new Inwestor(new Losowa()) );
    }

    public void nowySMA() {
        inwestorzy.add( new Inwestor(new SMA()) );
    }

    public void odpytaj( SystemTransakcyjny system ) {
        for ( Inwestor i : inwestorzy )
            i.odpytaj(system);
    }

    public void wypiszRezultat() {
        for ( Inwestor inwestor : inwestorzy )
            inwestor.wypiszRezultat();
    }

    // Metoda potrzebna do testów.
    public int sumaGotowki() {
        int suma = 0;
        for ( Inwestor i : inwestorzy )
            suma += i.gotowka();

        return suma;
    }

    // Metoda potrzebna do testów.
    public int sumaAkcji( Spolka spolka ) {
        int suma = 0;
        for ( Inwestor i : inwestorzy )
            suma += i.liczbaAkcji(spolka);

        return suma;
    }
}
