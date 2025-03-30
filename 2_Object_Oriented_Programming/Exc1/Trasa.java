package tramwaje;

import java.util.Arrays;

public class Trasa {
    private Przystanek[] przystanki;
    private int[] czasy_dojazdu;
    private int size;

    public Trasa() {
        size = 0;
        this.przystanki = new Przystanek[0];
        this.czasy_dojazdu = new int[0];
    }

    /*********************** Metody public ************************/

    public void wypisz() {
        for ( int i = 0; i < size; i++ ) {
            System.out.println(
                    przystanki[i].toString() + " " +
                    czasy_dojazdu[i] + "\n");
        }
    }

    public int ilePrzystankow() { return size; }

    public Przystanek przystanek( int idx ) { return przystanki[idx]; }

    public boolean czyKoniec( int idx, boolean kierunek ) {
        if ( kierunek )
            return idx == size - 1;
        return idx == 0;
    }

    public void dodajPrzystanek( Przystanek przystanek, int czas ) {
        if( size >= przystanki.length )
            powieksz();
        przystanki[size] = przystanek;
        czasy_dojazdu[size] = czas;
        size++;
    }

    public int czasTrasy() {
        int res = 0;
        for ( int i = 0; i < size; i++ ) {
            res += 2 * czasy_dojazdu[i];
        }

        return res;
    }


    // Zwraca następny przystanek na trasie dla podanego indeksu
    public Przystanek nastepnyPrzystanek( int idx, boolean kierunek ) {
        if ( kierunek ) {
            if ( idx == size - 1 )
                return przystanki[idx];
            else
                return przystanki[idx + 1];
        }
        else {
            if ( idx == 0 )
                return przystanki[idx];
            else
                return przystanki[idx - 1];
        }
    }

    // Zwraca czas dojazdu do następnego przystanku dla podanego indeksu
    public int zaIleNastepny( int idx, boolean kierunek ) {
        if ( kierunek ) {
            if ( idx == size - 1 )
                return czasy_dojazdu[size - 1];

            return czasy_dojazdu[idx];
        }
        else {
            if ( idx == 0 )
                return czasy_dojazdu[size - 1];

            return czasy_dojazdu[idx - 1];
        }
    }

    public Przystanek losujPrzystanek( int idx, boolean kierunek ) {
        if ( size <= 0 )
            return null;
        int rand;
        if ( kierunek )
            rand = Losowanie.losuj( idx + 1, size - 1 );
        else
            rand = Losowanie.losuj( 0, idx - 1 );

        return przystanki[rand];
    }

    public void clean() {
        for ( int i = 0; i < size; i++ )
            przystanki[i].clean();
    }

    /*********************** Metody private ************************/

    private void powieksz() {
        przystanki = Arrays.copyOf( przystanki, przystanki.length * 2 + 2 );
        czasy_dojazdu = Arrays.copyOf( czasy_dojazdu, czasy_dojazdu.length * 2 + 2 );
    }
}
